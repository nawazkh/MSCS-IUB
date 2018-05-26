
# coding: utf-8

# In[1]:


import librosa
import numpy as np

def extract_feature(file_name):
    X, sample_rate = librosa.load(file_name)
    stft = np.abs(librosa.stft(X))
    mfccs = np.mean(librosa.feature.mfcc(y=X, sr=sample_rate, n_mfcc=40).T,axis=0)
    chroma = np.mean(librosa.feature.chroma_stft(S=stft, sr=sample_rate).T,axis=0)
    mel = np.mean(librosa.feature.melspectrogram(X, sr=sample_rate).T,axis=0)
    contrast = np.mean(librosa.feature.spectral_contrast(S=stft, sr=sample_rate).T,axis=0)
    tonnetz = np.mean(librosa.feature.tonnetz(y=librosa.effects.harmonic(X),
    sr=sample_rate).T,axis=0)
    return mfccs,chroma,mel,contrast,tonnetz


# In[2]:


import glob as glob
import ntpath

def parse_files(dirname, ext = '*.wav'):
    features, labels = np.empty((0, 194)), np.empty(0)
    for wavf in glob.glob(dirname+ext):
        mfccs, chroma, mel, contrast, tonnetz = extract_feature(wavf)
        ext_features = np.hstack([mfccs, chroma, mel, contrast, tonnetz])
        filename = ntpath.basename(wavf).split('-')[0]
        if filename == 'Truck':
            print 'Appending 1 for Truck'
            ext_features = np.append(ext_features, 1)
        else:
            print 'Appending 2 for non-Truck'
            ext_features = np.append(ext_features, 2)
        features = np.vstack([features, ext_features])
        print 'Features shape: ', features.shape
    return np.array(features)


# In[3]:


from sklearn.preprocessing import Imputer
# temp_data_set = '/home/subhojit/temp/truck-audioset/noisy-data-set/temp/'
# features = parse_files(temp_data_set)
# Z = Imputer().fit_transform(features)
# print 'Z.shape: ', Z.shape
# np.savetxt('/home/subhojit/temp/truck-audioset/noisy-data-set/temp/features.csv', Z, delimiter=',')

truck_audio     = '/home/subhojit/temp/truck-audioset/noisy-data-set/truck/'
non_truck_audio = '/home/subhojit/temp/truck-audioset/noisy-data-set/non_truck/'

truck_features = parse_files(truck_audio)
nontruck_features = parse_files(non_truck_audio)

Z = Imputer().fit_transform(truck_features)
print 'Truck audio features shape: ', Z.shape
np.savetxt('/home/subhojit/temp/truck-audioset/noisy-data-set/truck_features.csv', Z, delimiter=',')

Z = Imputer().fit_transform(nontruck_features)
print 'Non Truck audio features shape: ', Z.shape
np.savetxt('/home/subhojit/temp/truck-audioset/noisy-data-set/non_truck_features.csv', Z, delimiter=',')


# In[4]:


import pandas as pd


# csvfile = '/home/subhojit/temp/truck-audioset/noisy-data-set/temp/features.csv'
# sound_data_set = pd.read_csv(csvfile, sep = ',', header=None) #read_csv not working

# sound_data_set.iloc[:]

truck_data_csv      = '/home/subhojit/temp/truck-audioset/noisy-data-set/truck_features.csv'
non_truck_data_csv  = '/home/subhojit/temp/truck-audioset/noisy-data-set/non_truck_features.csv'

truck_snd_dataset      =  pd.read_csv(truck_data_csv,sep = ',', header=None )
non_truck_snd_dataset  =  pd.read_csv(non_truck_data_csv,sep = ',', header=None )


# In[5]:


truck_snd_dataset.iloc[:].head()


# In[6]:


non_truck_snd_dataset.iloc[:].head()


# In[8]:


frames = [truck_snd_dataset, non_truck_snd_dataset]
merged_data = pd.concat(frames)
#shuffle rows
# merged_data : (1200, 194)
merged_data = merged_data.sample(frac=1).reset_index(drop=True)

# labels = sound_data_set.iloc[:, 193]
# labels.apply(np.int16)
# labels.iloc[:]
merged_data = merged_data.iloc[:, : 194]
#extract labels 

labels = merged_data.iloc[:,193]
labels = labels.apply( np.int16)#
# print merged_data.shape # (1200, 194)
merged_data.iloc[:].head()
# print labels.shape # (1200,)
# labels.iloc[:].head()


# In[10]:


#removing last column containing labels from dataset
print type(merged_data)

dataset = merged_data.drop(193, axis=1)
dataset.iloc[:].head()


# In[11]:


#, dataset, labels ;;
from sklearn.model_selection import train_test_split
tr_features, ts_features, tr_labels, ts_labels = train_test_split(dataset, labels)

# print 'dataset.shape: ', dataset.shape             #=> (1200, 193)
# print 'labels.shape', labels.shape                 #=> (1200,)
# print 'tr_features.shape: ',tr_features.shape      #=> (900, 193)
# print 'tr_labels.shape: ',tr_labels.shape          #=> (900,)
# print 'ts_features.shape: ',ts_features.shape      #=> (300, 193)
# print 'ts_labels.shape: ',ts_labels.shape          #=> (300,)


# In[12]:


from sklearn.preprocessing import StandardScaler
scaler = StandardScaler()
# Fit only to the training data
scaler.fit(tr_features)

tr_features = scaler.transform(tr_features)
ts_features = scaler.transform(ts_features)


# In[13]:


from sklearn.neural_network import MLPClassifier
mlp = MLPClassifier(hidden_layer_sizes=(10,10,10), max_iter= 500, verbose=True)

mlp.fit(tr_features, tr_labels)


# In[15]:


predictions = mlp.predict(ts_features)
c = 0
for a,b in zip(predictions,ts_labels):
#     print 'Prediction: ',a, ' Actual: ', b
    if a == b:
        c+=1
print 'Correct: ', c
print 'Passed: ', 100*c/len(predictions)

