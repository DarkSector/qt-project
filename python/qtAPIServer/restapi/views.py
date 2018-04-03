# Create your views here.
import json

from rest_framework.exceptions import ValidationError
from rest_framework.response import Response
from rest_framework.views import APIView
from api import Histogram, HaarCascadeFaceDetection, ImageNotFoundException


class GenerateHistogramView(APIView):

    def post(self, request):
        # get the image location
        image_location = request.data.get('image_location')
        # use opencv to create a histogram and return results
        histogram = Histogram(image_location, 'rgb')
        data = histogram.generate()
        return Response({'foo': 'bar'})


class FaceDetection(APIView):

    def post(self, request):
        image_location = request.data.get('image_location')
        try:
            hcfd = HaarCascadeFaceDetection(image_location)
            features = hcfd.detect()
        except ImageNotFoundException:
            return Response({"error": True, "data": "Image not found"})
        else:
            if not isinstance(features, tuple):
                if features.size != 0:
                    return Response({"error": False, "data": features.tolist()})
                # because validation error sends back a 400
            return Response({"error": True, "data": "No faces found"})


class SanityCheck(APIView):

    def get(self, request):
        return Response("ok")
