from rest_framework.response import Response
from rest_framework.decorators import api_view

from base.serializer import AudioSerializer
from base.models import Audiofile
import HMM.recognition

@api_view(['GET'])
def getData(request):
    data  = Audiofile.objects.all()
    audioSerializer = AudioSerializer(data,many=True)
    return Response(audioSerializer.data)
    
@api_view(['POST'])
def receiveData(request):
    audioSerializer = AudioSerializer(data=request.data)
    if audioSerializer.is_valid():
        files = request.FILES.get('audio')
        function = HMM.recognition.validate(files)
        intent = HMM.recognition.intent(function)
        audioSerializer.save();
        return Response(intent)
    return Response(audioSerializer.errors)

@api_view(['POST'])
def receiveESP(request):
    print(request)
    return Response({'message':'response'})