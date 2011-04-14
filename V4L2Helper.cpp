#include "V4L2Helper.h"

// ====================================================================================================
// static
std::string V4L2Helper::ToString(v4l2_colorspace p_colorSpace)
{
    switch (p_colorSpace)
    {
	case V4L2_COLORSPACE_SMPTE170M:
	    return "V4L2_COLORSPACE_SMPTE170M";
	    break;
	case V4L2_COLORSPACE_SMPTE240M:
	    return "V4L2_COLORSPACE_SMPTE240M";
	    break;
	case V4L2_COLORSPACE_REC709:
	    return "V4L2_COLORSPACE_REC709";
	    break;
	case V4L2_COLORSPACE_BT878:
	    return "V4L2_COLORSPACE_BT878";
	    break;
	case V4L2_COLORSPACE_470_SYSTEM_M:
	    return "V4L2_COLORSPACE_470_SYSTEM_M";
	    break;
	case V4L2_COLORSPACE_470_SYSTEM_BG:
	    return "V4L2_COLORSPACE_470_SYSTEM_BG";
	    break;
	case V4L2_COLORSPACE_JPEG:
	    return "V4L2_COLORSPACE_JPEG";
	    break;
	case V4L2_COLORSPACE_SRGB:
	    return "V4L2_COLORSPACE_SRGB";
	    break;
	default:
	    return "Unknown ColorSpace";
    }
}

// ====================================================================================================
// static
std::string V4L2Helper::ToString(v4l2_field p_fieldType)
{
    switch (p_fieldType)
    {
	case V4L2_FIELD_ANY:
	    return "V4L2_FIELD_ANY";
	    break;
	case V4L2_FIELD_NONE:
	    return "V4L2_FIELD_NONE";
	    break;
	case V4L2_FIELD_TOP:
	    return "V4L2_FIELD_TOP";
	    break;
	case V4L2_FIELD_BOTTOM:
	    return "V4L2_FIELD_BOTTOM";
	    break;
	case V4L2_FIELD_INTERLACED:
	    return "V4L2_FIELD_INTERLACED";
	    break;
	case V4L2_FIELD_SEQ_TB:
	    return "V4L2_FIELD_SEQ_TB";
	    break;
	case V4L2_FIELD_SEQ_BT:
	    return "V4L2_SEQ_BT";
	    break;
	case V4L2_FIELD_ALTERNATE:
	    return "V4L2_FIELD_ALTERNATE";
	    break;
	default:
	    return "Unknown Field Type";
    }
}
