package org.fjank.imbs;

import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.annotation.ByVal;
import org.bytedeco.javacpp.annotation.Cast;
import org.bytedeco.javacpp.annotation.Platform;
import org.bytedeco.javacpp.opencv_core;

/**
 * This file contains the JavaCPP based JavaCV BackgroundSubtractor implementation of IMBS, directly pluggable into
 * JavaCV.
 * @author Frank Karlstrøm - frank.karlstrom@gmail.com - fk@databaseforum.no
 */

@Platform(include = {"imbs.hpp"},
        includepath = {
                "C:/dev/opencv/build/",
                "C:/dev/opencv/modules/core/include/",
                "C:/dev/opencv/modules/highgui/include/",
                "C:/dev/opencv/modules/imgcodecs/include/",
                "C:/dev/opencv/modules/videoio/include/",
                "C:/dev/opencv/modules/imgproc/include/",
                "C:/dev/opencv/modules/features2d/include/",
                "C:/dev/opencv/modules/flann/include/"
        },
        linkpath = {"C:/dev/opencv/build/lib/Release"},
        link = {"imbs_310", "opencv_core310", "opencv_imgproc310"},
        library = "jni_imbs_310"
)
public class Imbs {
    /** The java implementation of IMBS Background Subtractor. Works the same way as OpenCV Bckground substractors. */
    public static class BackgroundSubtractorIMBS extends Pointer {
        static {
            Loader.load();
        }

        /**
         * Default constructor, using sensible default values.
         * <pre><ul><li>fps = 25
         * <li>fgThreshold = 15
         * <li>associationThreshold = 5
         * <li>samplingPeriod = 400 (ms)
         * <li>minBinHeight = 2
         * <li>numSamples = 30
         * <li>alpha = 0.65
         * <li>beta = 1.15
         * <li>tau_s = 60
         * <li>tau_h = 40
         * <li>minArea = 30
         * <li>persistencePeriod = samplingPeriod * numSamples / 3.0 (ms)
         * <li>morphologicalFiltering = false
         * </ul>
         * </pre>
         */
        public BackgroundSubtractorIMBS() {
            allocate();
        }

        /** The full constructor. For sensible defaults, see the default constructor. */
        public BackgroundSubtractorIMBS(double fps,
                int fgThreshold,
                int associationThreshold,
                double samplingPeriod,
                int minBinHeight,
                int numSamples,
                double alpha,
                double beta,
                double tau_s,
                double tau_h,
                double minArea,
                double persistencePeriod,
                boolean morphologicalFiltering) {
            allocate(fps, fgThreshold, associationThreshold, samplingPeriod, minBinHeight, numSamples, alpha, beta,
                    tau_s, tau_h, minArea, persistencePeriod, morphologicalFiltering);


        }

        private native void allocate(double fps,
                int fgThreshold,
                int associationThreshold,
                double samplingPeriod,
                int minBinHeight,
                int numSamples,
                double alpha,
                double beta,
                double tau_s,
                double tau_h,
                double minArea,
                double persistencePeriod,
                @Cast("bool") boolean morphologicalFiltering);

        /**
         * Computes a foreground mask.
         * @param image Next video frame.
         * @param fgmask The output foreground mask as an 8-bit binary image.
         * @param learningRate The value between 0 and 1 that indicates how fast the background model is learnt.
         * Negative parameter value makes the algorithm to use some automatically chosen learning rate. 0 means that the
         * background model is not updated at all, 1 means that the background model is completely reinitialized from
         * the last frame.
         */
        public native void apply(@ByVal opencv_core.Mat image, @ByVal opencv_core.Mat fgmask,
                double learningRate/*=-1*/);

        public native void apply(@ByVal opencv_core.Mat image, @ByVal opencv_core.Mat fgmask);

        /** @return true if the background model is ready for processing, false otherwise. */
        public native @Cast("bool") boolean isReady();

        /**
         * computes a background image which shows only the highest bin for each pixel*
         * Sometimes the background image can be very blurry, as it contain the average background
         * statistics.
         * @param backgroundImage The output background image.
         */
        public native void getBackgroundImage(@ByVal opencv_core.Mat backgroundImage);

        private native void allocate();
    }
}
