# Create your views here.
import json
from rest_framework.response import Response
from rest_framework.views import APIView
from api import Histogram


class GenerateHistogramView(APIView):

    def post(self, request):
        # get the image location
        image_location = request.data.get('image_location')

        # use opencv to create a histogram and return results
        histogram = Histogram(image_location, 'rgb')
        data = histogram.generate()
        return Response(json.dumps({'foo': 'bar'}))

    def get(self, request):
        return Response({'foo': 'bar'})


class FaceDetection(APIView):

    def post(self, request):

        pass