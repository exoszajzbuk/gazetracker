#include "Webcamera.h"
#include "V4L2Helper.h"

#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sstream>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

using namespace std;

// ====================================================================================================
static void errno_exit(const std::string& p_error)
{
        cerr << p_error << " error: " << errno << ", " << strerror(errno) << endl;

	exit (EXIT_FAILURE);
}

// ====================================================================================================
WebCamera::WebCamera(const std::string& p_devName, int width, int height) :
    m_initialized(false),
    m_width(0),
    m_height(0),
    m_colorSpace(V4L2_COLORSPACE_SMPTE170M),
    m_fieldType(V4L2_FIELD_ANY),
    m_devName(p_devName),
    m_fd(-1),
    m_buffers(NULL),
    m_numOfBuffers(0),
    m_buf(NULL)
{
    m_user_width = width;
    m_user_height = height;
}

// ====================================================================================================
void WebCamera::Init()
{
    if (m_initialized)
    {
	cerr << "Warning: Device already initialized" << endl;
	return;
    }

    OpenDevice();

    InitDevice();

    m_initialized = true;
}

// ====================================================================================================
void WebCamera::Close()
{
    if (!m_initialized)
    {
	cerr << "Warning: Device is not initialized but we are trying to uninitialize it" << endl;
	return;
    }

    UnInitDevice();

    CloseDevice();

    m_buf = NULL;
    m_initialized = false;
}

// ====================================================================================================
WebCamera::~WebCamera()
{
    Close();
}

// ====================================================================================================
void WebCamera::OpenDevice()
{
    struct stat st;

    if (stat(m_devName.c_str(), &st) == -1) {
	cerr << "Cannot identify '" << m_devName << "':" << errno << ", " << strerror(errno) << endl;
	exit (EXIT_FAILURE);
    }

    if (! S_ISCHR(st.st_mode)) {
	cerr << m_devName << " is no device" << endl;
	exit (EXIT_FAILURE);
    }

    m_fd = open(m_devName.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);

    if (m_fd == -1)
    {
	cerr << "Cannot open '" << m_devName << "': " << errno << ", " << strerror(errno) << endl;
	exit (EXIT_FAILURE);
    }
}

// ====================================================================================================
void WebCamera::InitDevice()
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

    if (-1 == Xioctl(m_fd, VIDIOC_QUERYCAP, &cap))
    {
	if (EINVAL == errno)
	{
	    cerr << m_devName << " is no V4L2 device" << endl;
	    exit (EXIT_FAILURE);
	}
	else
	    errno_exit ("VIDIOC_QUERYCAP");
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
	cerr << m_devName << " is no video capture device" << endl;
	exit(EXIT_FAILURE);
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
	cerr << m_devName << " does not support streaming i/o" << endl;
	exit (EXIT_FAILURE);
    }

    /* Select video input, video standard and tune here. */


    CLEAR (cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == Xioctl (m_fd, VIDIOC_CROPCAP, &cropcap)) {
	crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	crop.c = cropcap.defrect; /* reset to default */

	if (-1 == Xioctl (m_fd, VIDIOC_S_CROP, &crop)) {
	    switch (errno) {
		case EINVAL:
		    /* Cropping not supported. */
		    break;
		default:
		    /* Errors ignored. */
		    break;
	    }
	}
    } else {	
	/* Errors ignored. */
    }


    CLEAR (fmt);

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = m_user_width;
    fmt.fmt.pix.height      = m_user_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_SRGB;

    if (-1 == Xioctl (m_fd, VIDIOC_S_FMT, &fmt))
	errno_exit ("VIDIOC_S_FMT");

    m_height = fmt.fmt.pix.height;
    m_width = fmt.fmt.pix.width;

    m_fieldType = fmt.fmt.pix.field;
    m_colorSpace = fmt.fmt.pix.colorspace;

    /* Note VIDIOC_S_FMT may change width and height. */

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
	fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
	fmt.fmt.pix.sizeimage = min;

    InitMMap();
}

// ====================================================================================================
int WebCamera::Xioctl(int p_fd, int p_request, void *p_arg)
{
        int r;

        do r = ioctl (p_fd, p_request, p_arg);
        while (-1 == r && EINTR == errno);

        return r;
}

// ====================================================================================================
void WebCamera::InitMMap()
{
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    if (Xioctl (m_fd, VIDIOC_REQBUFS, &req) == -1)
    {
	if (EINVAL == errno)
	{
	    cerr << m_devName << " does not support memory mapping" << endl;
	    exit (EXIT_FAILURE);
	}
	else
	    errno_exit ("VIDIOC_REQBUFS");
    }

    if (req.count < 2) {
	cerr << "Insufficient buffer memory on " << m_devName << endl;
	exit (EXIT_FAILURE);
    }

    m_buffers = new Buffer[req.count]; //(buffer*)calloc (req.count, sizeof (*m_buffers));

    if (!m_buffers) {
	cerr << "Out of memory" << endl;
	exit (EXIT_FAILURE);
    }

    m_numOfBuffers = req.count;

    for (unsigned i = 0; i < req.count; ++i)
    {
	struct v4l2_buffer buf;

	CLEAR (buf);

	buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory      = V4L2_MEMORY_MMAP;
	buf.index       = i;

	if (-1 == Xioctl (m_fd, VIDIOC_QUERYBUF, &buf))
	    errno_exit ("VIDIOC_QUERYBUF");

	m_buffers[i].length = buf.length;
	m_buffers[i].start =
	    mmap (NULL /* start anywhere */,
		    buf.length,
		    PROT_READ | PROT_WRITE /* required */,
		    MAP_SHARED /* recommended */,
		    m_fd, buf.m.offset);

	if (MAP_FAILED == m_buffers[i].start)
	    errno_exit ("mmap");
    }
}

// ====================================================================================================
void WebCamera::StartCapturing()
{
    enum v4l2_buf_type type;

    for (unsigned i = 0; i < m_numOfBuffers; ++i)
    {
	struct v4l2_buffer buf;

	CLEAR (buf);

	buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory      = V4L2_MEMORY_MMAP;
	buf.index       = i;

	if (Xioctl (m_fd, VIDIOC_QBUF, &buf) == -1)
	    errno_exit ("VIDIOC_QBUF");
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (Xioctl (m_fd, VIDIOC_STREAMON, &type) == -1)
	errno_exit ("VIDIOC_STREAMON");

}

// ====================================================================================================
const void* WebCamera::NextFrame()
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO (&fds);
    FD_SET (m_fd, &fds);

    /* Timeout. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    r = select (m_fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r)
    {
	if (EINTR == errno)
	    cout << "Error" << endl;

	errno_exit ("select");
    }

    if (0 == r)
    {
	cerr <<  "select timeout" << endl;
	exit (EXIT_FAILURE);
    }

    return ReadFrame();
}

// ====================================================================================================
const void * WebCamera::ReadFrame()
{
    if (m_buf)
    {
	if (-1 == Xioctl (m_fd, VIDIOC_QBUF, m_buf))
	    errno_exit ("VIDIOC_QBUF");
	delete m_buf;
    }

    m_buf = new v4l2_buffer();

    CLEAR (*m_buf);

    m_buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_buf->memory = V4L2_MEMORY_MMAP;

    if (-1 == Xioctl(m_fd, VIDIOC_DQBUF, m_buf))
    {
	switch (errno)
	{
	    case EAGAIN:
		cout << "Error in ReadFrame" << endl;
		return 0;

	    case EIO:
		/* Could ignore EIO, see spec. */

		/* fall through */

	    default:
		errno_exit ("VIDIOC_DQBUF");
	}
    }

    assert(m_buf->index < m_numOfBuffers);

    return m_buffers[m_buf->index].start;
}

// ====================================================================================================
void WebCamera::StopCapturing()
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (Xioctl (m_fd, VIDIOC_STREAMOFF, &type) == -1)
	errno_exit ("VIDIOC_STREAMOFF");
}

// ====================================================================================================
void WebCamera::UnInitDevice()
{
    for (unsigned i = 0; i < m_numOfBuffers; ++i)
	if (-1 == munmap (m_buffers[i].start, m_buffers[i].length))
	    errno_exit("munmap");

    delete m_buffers;
}

// ====================================================================================================
void WebCamera::CloseDevice()
{
    if (-1 == close(m_fd))
	errno_exit ("close");

    m_fd = -1;
}

// ====================================================================================================
unsigned WebCamera::GetWidth() const
{
    return m_width;
}

// ====================================================================================================
unsigned WebCamera::GetHeight() const
{
    return m_height;
}

// ====================================================================================================
std::string WebCamera::GetColorSpaceString() const
{
    if (!m_initialized)
	return "UnInitialized";

    return V4L2Helper::ToString(m_colorSpace);
}

// ====================================================================================================
std::string WebCamera::GetFieldTypeString() const
{
    if (!m_initialized)
	return "UnInitialized";

    return V4L2Helper::ToString(m_fieldType);
}

// ====================================================================================================
void WebCamera::SetDeviceName(const std::string& p_devName)
{
    cout << "Setting webcam device name to: " << p_devName << endl;
    m_devName = p_devName;
}

// ====================================================================================================
std::string WebCamera::GetInformation() const
{
    stringstream r;

    r << "dev:" << m_devName;
    r << "w:" << GetWidth();
    r << ",h:" << GetHeight();
    r << ",CS:" << GetColorSpaceString();
    r << ",FT:" << GetFieldTypeString();

    return r.str();
}

// ====================================================================================================
std::string WebCamera::GetStreamerName() const
{
    return "WebCamera";
}
