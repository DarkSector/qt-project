# Create your views here.
from rest_framework.response import Response
from rest_framework.views import APIView


class GenerateHistogramView(APIView):

    def post(self, request, histogram_type):

        return Response()

    def get(self, request, histogram_type):
        return Response({'foo': 'bar'})
