import numpy as np
import cv2


class Histogram(object):
    bins = 128

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
        hist = np.histogram([self.image], self.bins, [0, 256, 0, 256, 0, 256],)

        # return out 3D histogram as a flattened array
        return hist.flatten()

    def show_image(self):
        cv2.imshow('My Image', self.image)
        cv2.waitKey(0) & 0xFF
        cv2.destroyAllWindows()


histogram = Histogram('/Users/DarkSector/Desktop/img_0001.jpg', 'rgb')
histogram.show_image()
histogram.generate()