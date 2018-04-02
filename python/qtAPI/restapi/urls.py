from django.conf.urls import url
from django.urls import path

from . import views

urlpatterns = [
    path('histogram/<str:histogram_type>', views.GenerateHistogramView.as_view()),
]