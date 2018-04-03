import numpy as np
import cv2


class ImageNotFoundException(Exception):
    def __init__(self, message):
        super(ImageNotFoundException, self).__init__(message)


class Histogram(object):
    bins = [8, 8, 8]

    def __init__(self, image_location, histogram_type='rgb'):
        self.image_location = image_location
        self.histogram_type = histogram_type

    @property
    def image(self):
        return cv2.imread(self.image_location)

    def generate(self):
        # b 0 , g 1, r 2
        # mask is None
        # bins are 128 by default
        hist_list = np.array([])
        for channel in range(3):
            _hist = cv2.calcHist([self.image], [channel], None, [8], [0, 256])
            hist = cv2.normalize(_hist)

        # return out 3D histogram as a flattened array
        return hist.flatten()

    def show_image(self):
        cv2.imshow('My Image', self.image)
        cv2.waitKey(0) & 0xFF
        cv2.destroyAllWindows()


class HaarCascadeFaceDetection(object):

    def __init__(self, image_location):
        self.image_location = image_location

    @property
    def image(self):
        img = cv2.imread(self.image_location, cv2.IMREAD_COLOR)
        if img is None:
            raise ImageNotFoundException("Unable to find image in location")
        return img

    @property
    def face_cascade_classifier(self):
        return None

    def _detect_faces(self):
        pass

