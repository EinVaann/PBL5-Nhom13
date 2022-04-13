from dataclasses import field
from distutils.command.upload import upload
from .models import Audiofile
from pkg_resources import require
from rest_framework import serializers

class AudioSerializer(serializers.ModelSerializer):
    class Meta:
        model=Audiofile
        fields='__all__'