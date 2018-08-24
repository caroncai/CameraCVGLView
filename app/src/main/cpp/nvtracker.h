#ifndef NV_TRACKER_H_
#define NV_TRACKER_H_
#include <thread>
#include <condition_variable>
#include <opencv2/highgui.hpp>
#include <FaceTracker/Tracker.h>


namespace nv
{
    class NVApp;
    namespace tracker
    {
        class NVTracker
        {
        public:
            struct Image{
                unsigned char* buf_;
                int width_;
                int height_;
                double timestamp_;

                Image():width_(640),
                        height_(480),
                        buf_(0),
                        timestamp_(0.0)
                {

                }
            };


            NVTracker(NVApp *app, const std::string& path);

            ~NVTracker();

            void Resume();

            void Pause();

            void NotifyCameraReady();

            void NotifyCameraWait();

            bool PushImage(int width, int height, unsigned char* buf, double timestamp);

            bool PopImage(Image& image);

            void _Run();

            void Destroy();

        protected:

            bool _Capture(cv::Mat& gray);

            void _WaitForCamReady();

            void _ProcessIO(const std::string& path);

            void _PopImage(const cv::Mat& image);

            void _DrawOnImage(cv::Mat &image, cv::Mat &shape, cv::Mat &visi)
            {
                int i,n = shape.rows/2; cv::Point p1,p2; cv::Scalar c;

                //draw points
                for(i = 0; i < n; i++)
                {
                    if(visi.at <int>(i,0) == 0)continue;
                    p1 = cv::Point(shape.at<double>(i,0), shape.at<double>(i+n,0));
                    c = CV_RGB(0,0,255); cv::circle(image,p1,3,c);
                }return;
            }

        private:

            enum TrackerMessage{
                MSG_NONE = 0,
                MSG_WAIT_READY,
                MSG_FRAME_AVAIABLE,
                MSG_LOOP_EXIT
            };

            std::mutex msg_mut_;


            std::mutex pc_mut_;
            std::condition_variable pc_cond_;

            std::mutex tl_mut_;
            std::condition_variable tl_cond_;

            static int kMaxImages;

            NVApp        *app_;

            std::string  app_path_;
            enum TrackerMessage msg_;
            Image       images_[2];
            Image       pop_image_;

            cv::Mat mat_list_[2];
            int     image_index_;

            bool        start_;
            bool        is_process_;
            bool        do_process_;
            bool        pop_;
            bool        is_pause_;

            double      timestamp_;
            int         last_image_index_;

            bool       cam_configured_;

            // Related args about tracker
            FACETRACKER::Tracker   *model_;

        };
    }
}



#endif //NV_TRACKER_H_