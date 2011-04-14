#ifndef __WEBCAM_WEBCAMERA_H__
#define __WEBCAM_WEBCAMERA_H__

#include <string>

#include <linux/videodev2.h>

    struct Buffer {
	void *                  start;
	size_t                  length;
    };

    class WebCamera
    {
	public:
	    WebCamera(const std::string& p_devName = "", const int width = 800, const int height = 600);
	    ~WebCamera();

	    void Init();
	    void Close();

	    void StartCapturing();
	    const void* NextFrame();
	    void StopCapturing();

	    void SetDeviceName(const std::string& p_devName);

	    unsigned GetWidth() const;
	    unsigned GetHeight() const;

	    std::string GetColorSpaceString() const;
	    std::string GetFieldTypeString() const;

	    std::string GetInformation() const;
	    virtual std::string GetStreamerName() const;

	private:
	    void OpenDevice();
	    void InitDevice();
	    void InitMMap();
	    void UnInitDevice();
	    void CloseDevice();

	    int Xioctl(int p_fd, int p_request, void *p_arg);
	    const void* ReadFrame();

	    bool		m_initialized;

	    unsigned		m_width;
	    unsigned		m_height;

        int     m_user_width;
        int     m_user_height;

	    v4l2_colorspace	m_colorSpace;
	    v4l2_field		m_fieldType;

	    std::string		m_devName;	    /// Name of the device
	    int			m_fd;		    /// File descriptor of the device
	    Buffer*		m_buffers;
	    unsigned		m_numOfBuffers;
	    struct v4l2_buffer* m_buf;
    };

#endif
