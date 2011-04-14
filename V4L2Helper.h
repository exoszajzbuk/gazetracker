#ifndef __WEBCAM_V4L2HELPER_H__
#define __WEBCAM_V4L2HELPER_H__

#include <linux/videodev2.h>
#include <string>

    class V4L2Helper
    {
	public:
	    static std::string ToString(v4l2_colorspace p_colorSpace);

	    static std::string ToString(v4l2_field p_fieldType);
    };

#endif
