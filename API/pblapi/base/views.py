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
    if request.FILES:
        files = request.FILES.get('audio')
        function = HMM.recognition.validate(files)
        return Response({'message':function})
    return Response({'message':'error'})
    # audioSerializer = AudioSerializer(data=request.data)
    # if audioSerializer.is_valid():
    #     files = request.FILES.get('audio')
    #     function = HMM.recognition.validate(files)
    #     # audioSerializer.save()
    #     return Response({'message':function})
    # return Response(audioSerializer.errors)

