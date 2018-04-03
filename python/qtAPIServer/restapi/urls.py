from django.conf.urls import url
from .views import GenerateHistogramView, FaceDetection

urlpatterns = [
    url('^histogram', GenerateHistogramView.as_view(), name='generate_histogram'),
    url('^face_detection', FaceDetection.as_view(), name='detect_faces')
]
