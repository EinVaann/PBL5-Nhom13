from rest_framework.response import Response
from rest_framework.decorators import api_view

from base.serializer import AudioSerializer
from base.models import Audiofile
import HMM.recognition
from members.models import Device

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

@api_view(['GET'])
def getStatus(request, pk):
    device = Device.objects.get(id=pk)
    return Response({'name':device.name,'light':device.light,'fan':device.fan,'music':device.music})

