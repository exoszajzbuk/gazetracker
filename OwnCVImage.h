////////////////////////////////////////////////////////////////////////////////
// Filename: OwnCVImage.h
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Header of OwnCVImage class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __OwnCVImage__
#define __OwnCVImage__

// OpenCV
#include <cv.h>


////////////////////////////////////////////////////////////
// Class name:  OwnCVImage
//
// Description: OwnCVImage contains a CV image and the
//              state of the image is changed or not
////////////////////////////////////////////////////////////
class OwnCVImage
{
    public:
        // default constructor
        OwnCVImage();
        
        // get image reference
        const cv::Mat&  getImage();

        // set image
        void            setImage(const cv::Mat& image);

        // get/set if image is changed
        bool            getChanged();
        void            setChanged(bool changed);

    private:
        // private variables for image and changed flag
        cv::Mat     m_image;
        bool        m_changed;
};

#endif
