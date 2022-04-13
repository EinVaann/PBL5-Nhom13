import math

import librosa
import numpy as np
import os
import pickle

model = []
file_name = [i for i in os.listdir('HMM/model') if i.endswith('.pkl')]
for f in file_name:
    file_paths = os.path.join('HMM/model/',f)
    with open(file_paths,'rb') as file:
        model.append(pickle.load(file))   

def get_mfcc(file_path):
    y, sr = librosa.load(file_path)  # read .wav file
    hop_length = math.floor(sr * 0.010)  # 10ms hop
    win_length = math.floor(sr * 0.025)  # 25ms frame
    # mfcc is 12 x T matrix
    mfcc = librosa.feature.mfcc(
        y=y, sr=sr, n_mfcc=12, n_fft=1024,
        hop_length=hop_length, win_length=win_length)
    # subtract mean from mfcc --> normalize mfcc
    mfcc = mfcc - np.mean(mfcc, axis=1).reshape((-1, 1))
    # delta feature 1st order and 2nd order
    delta1 = librosa.feature.delta(mfcc, order=1)
    delta2 = librosa.feature.delta(mfcc, order=2)
    # X is 36 x T
    X = np.concatenate([mfcc, delta1, delta2], axis=0) 
    # return T x 36 (transpose of X)
    return X.T  # hmmlearn use T x N matrix

def validate(audioFile):
    test_mfcc = get_mfcc(audioFile)
    scores = [model[i].score(test_mfcc) for i in range(len(file_name))]
    return file_name[np.argmax(scores)]
