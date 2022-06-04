from .models import Audiofile, HMM_model_file
from rest_framework import serializers
from django.contrib.auth.models import User

class AudioSerializer(serializers.ModelSerializer):
    class Meta:
        model=Audiofile
        fields='__all__'

class UserSerializer(serializers.ModelSerializer):
    models = serializers.PrimaryKeyRelatedField(many=True, queryset=HMM_model_file.objects.all())

    class Meta:
        model = User
        field = ['id', 'username' , 'models']