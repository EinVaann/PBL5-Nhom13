from .models import Audiofile
from rest_framework import serializers
from django.contrib.auth.models import User

class AudioSerializer(serializers.ModelSerializer):
    class Meta:
        model=Audiofile
        fields='__all__'