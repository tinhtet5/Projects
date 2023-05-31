
function varargout = Hearing_Aid(varargin)
% HEARING_AID MATLAB code for Hearing_Aid.fig
%      HEARING_AID, by itself, creates axes1 new HEARING_AID or raises the existing
%      singleton*.
%
%      H = HEARING_AID returns the handle to axes1 new HEARING_AID or the handle to
%      the existing singleton*.
%
%      HEARING_AID('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in HEARING_AID.M with the given input arguments.
%
%      HEARING_AID('Property','Value',...) creates axes1 new HEARING_AID or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Hearing_Aid_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Hearing_Aid_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Hearing_Aid

% Last Modified by GUIDE v2.5 29-Oct-2019 08:23:14

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Hearing_Aid_OpeningFcn, ...
                   'gui_OutputFcn',  @Hearing_Aid_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before Hearing_Aid is made visible.
function Hearing_Aid_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in axes1 future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Hearing_Aid (see VARARGIN)

% Choose default command line output for Hearing_Aid
handles.output = hObject;
set(handles.noise,'Enable','off')
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Hearing_Aid wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Hearing_Aid_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in axes1 future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in select.
function select_Callback(hObject, eventdata, handles)
% hObject    handle to select (see GCBO)
% eventdata  reserved - to be defined in axes1 future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
axes(handles.axes1)
global f_s;
f_s=44100;
[filename,path]=uigetfile('*.*','All Files(*.*)');
filess = strcat(path,filename);
disp(filess);
global y;
global Fs;
[y,Fs]=audioread(filess);
sound(y,Fs);
N=length(y);
t=(0:N-1)/f_s;
plot(t,y)
title('Original signal');
set(handles.noise,'Enable','on');



% --- Executes on button press in clear.
function clear_Callback(hObject, eventdata, handles)
% hObject    handle to clear (see GCBO)
% eventdata  reserved - to be defined in axes1 future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
axes(handles.axes1);
plot(0,0);
axes(handles.axes3);
plot(0,0);
axes(handles.axes2);
plot(0,0);
axes(handles.axes4);
plot(0,0);
axes(handles.axes5);
plot(0,0);
axes(handles.axes6);
plot(0,0);
axes(handles.axes7);
plot(0,0);
axes(handles.axes8);
plot(0,0);
clear sound;



% --- Executes on button press in noise.
function noise_Callback(hObject, eventdata, handles)
% hObject    handle to noise (see GCBO)
% eventdata  reserved - to be defined in axes1 future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

clear sound;
global y;
global Fs;
global f_s;
global z;
SNR=15;
z=awgn(y,SNR,'measured');
% z = y + (1*rands(1,length(y))');
sound(z,Fs);
N1=length(y);
t=(0:N1-1)/f_s;
axes(handles.axes3);
plot(t,z);
title('Noise interference');


% --- Executes on button press in filter.
function filter_Callback(hObject, eventdata, handles)
% hObject    handle to filter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clear sound;
global z;
global f_s;
global Fs;
global y;
N2=length(z);
t=(0:N2-1)/f_s;
y = Denoise(z);
sound(y,Fs);
axes(handles.axes5);
plot(t,y);
title('Denoised Signal');


% --- Executes on button press in fourier2.
function fourier2_Callback(hObject, eventdata, handles)
% hObject    handle to fourier2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global z;
global Fs;
c = fft(z);     % for plot the noise contained signal in frequency              
nfft = length(c);
c = c(1:nfft/2);
P2 = abs(c);
xfft = Fs*(0:nfft/2-1)/nfft;
axes(handles.axes4);
plot(xfft,P2,'r');          % end of plot the freq: domain signal
title('signal+noise in frequency domain')
guidata(hObject,handles);


% --- Executes on button press in fourier1.
function fourier1_Callback(hObject, eventdata, handles)
% hObject    handle to fourier1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global y;
global Fs;
c = fft(y);     % for plot the noise contained signal in frequency              
nfft = length(c);
c = c(1:nfft/2);
P2 = abs(c);
xfft = Fs*(0:nfft/2-1)/nfft;
axes(handles.axes2);
plot(xfft,P2,'r');          % end of plot the freq: domain signal
title('signal in frequency domain')


% --- Executes on button press in normalize.
function normalize_Callback(hObject, eventdata, handles)
% hObject    handle to normalize (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


%
function y = Denoise(x);
% y = denoiseEm(x);
% method to denoise a given signal using wavelets
% x is the input Matlab sound file
%THR is the threshold, SORH is for soft or hard thresholding, 
%KEEPAPP allows you to keep approximation coefficients
[thr,sorh,keepapp]=ddencmp('den' , 'wv' ,x);  
% returns a de-noised version xc of input signal x (our one-dimensional speech signal) 
%obtained by wavelet coefficients thresholding using global positive threshold THR
%PERF0 and PERFL2 are L2-norm recovery and compression score in percentage.
[y, cxc, lxc, perf0, perfl2] = wdencmp('gbl' ,x, 'db3' ,20,thr,sorh,keepapp);


function y = FreqShape(x,g,transitionV,fs)
% y = applySkiSlope(x,g,transitionV,fs)
% Creates the gain filter for a patient with moderate hearing loss.               
% The maximum gain will be g and the minimum gain will be one.  The magnitude                                                                          
% of gain function will be the concatenation of preset piecewise functions. 
% However the time of the transitions from one piecewise function to another can
% be set by the user in the elements of the tranisitonV.  The final frequency used
% will be fs/2 since that's the highest frequency that the input signal will contain.
% The output will be the filtered signal
% x - an input sound signal
% g - the maximum gain that will be applied to the signal
% transitionV - 4 element vector that has the values of where the gain changes
%               to the next piecewise function
% fs - the sampling frequency of the input signal
first = transitionV(1);
second = transitionV(2);
third = transitionV(3);
fourth = transitionV(4);
x_length = length(x);
n = nextpow2(x_length);
N = 2^n;
T = 1/fs;
X = fft(x,N);
gain = zeros(N,1);
% Sets the gain for the first stage of frequencies
firstC = (.3*(g-1))/first;
k=0;
while(k/N <= first/fs)
gain(k+1) = firstC*k/(N*T) + 1;
gain(N-k) = gain(k+1);
   k=k+1;
end;
% Sets the gain for the second stage of frequencies
secondC = firstC*first +1;    
secondC2 = (second-first)/5;
while(k/N <= second/fs)
gain(k+1) = 1 + (secondC-1)*exp(-((k/(N*T))-first)/secondC2);
gain(N-k) = gain(k+1);
   k=k+1;
end;
% Sets the gain for the third stage of frequencies
thirdC = 1 + (secondC-1)*exp(-second/secondC2);  
thirdC2 = (third-second)/5;
while(k/N <= third/fs)
gain(k+1) = g + (thirdC-g)*exp(-((k/(N*T)-second))/thirdC2);
gain(N-k) = gain(k+1);
   k=k+1;
end;
% Sets the gain for the fourth stage of frequencies
while(k/N <= fourth/fs)
gain(k+1) = g;
gain(N-k) = gain(k+1);
   k=k+1;
end;
% Sets the gain for the fifth stage of frequencies
fifthC = g;                
fifthC2 = (fs/2-fourth)/5;
while(k/N <= .5)
gain(k+1) = 1 + (fifthC-1)*exp(-((k/(N*T))-fourth)/fifthC2);
gain(N-k) = gain(k+1);
   k=k+1;
end; 
k_v = (0:N-1)/N;
%plot(k_v,gain);%entire filter transfer function
figure;%non-redundant filter transfer function
k_v = k_v*fs;
k_v = k_v(1:N/2+1);
plot(k_v,gain(1:N/2+1));
title('Frequency Shaper Transfer Function');
xlabel('Frequency (Hertz)');
ylabel('Gain');
Y = X+gain;
y = real(ifft(Y,N));
y = y(1:x_length);
t=[0:1/fs:(x_length-1)/fs];
figure;
plot(t,y,'r');
%figure;
%plot(t,x);


% --- Executes on button press in freq_sharp.
function freq_sharp_Callback(hObject, eventdata, handles)
% hObject    handle to freq_sharp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global Fs;
global y;
global f;
g=40;
transitionV=[1000 2000 3000 7000];
f = FreqShape(y,g,transitionV,Fs);
sound(y,Fs);
axes(handles.axes7);
N2=length(f);
t=(0:N2-1)/Fs;
plot(t,f);
title('Frequency-shaped signal');

function y = powerCompress(input, Psat,Fs)
% y = powerCompress(input, Psat,Fs)
% Takes in a a signal makes sure that the maximum power in any frequency
% is less than or equal to Psat.  Also had some denoising capabilities, by
% zeroing out very low power frequencies.
% input - input Matlab sound file
% Psat  - Saturation power
% FS    - Sampling frequency of the input signal
x=input;
len=Fs*0.1;
iter=floor(length(x)/len);
Plow=0.0008;
 
for rg=0:1:iter;
 start=rg*len+1;
 en= rg*len+len;
 if rg*len+len>length(x)
    en=length(x);
 end
 
clear signal X  X_pow Y_pow Y y z;
signal=x(start:en);
n = nextpow2(len);
N = 2^n;
X = fft(signal,N);
X_phase=angle(X);                % Save the old phase information
X_pow = abs(X)/N;
Y_pow = X_pow;
Y=zeros(N,1);
for k=0:N/2
   if Y_pow(k+1)<Plow              % Take out noise
      Y_pow(k+1)=0;
      Y_pow(N-k)=0;
   elseif Y_pow(k+1)>Psat          % Clip amplitudes higher than Psat
      Y_pow(k+1)=Psat;
      Y_pow(N-k)=Psat;
   end;
    Y(k+1) = Y_pow(k+1)*(cos(X_phase(k+1))+1i*sin(X_phase(k+1)));
    Y(N-k) = Y_pow(N-k)*(cos(X_phase(N-k))+1i*sin(X_phase(N-k)));
end;
y = real(ifft(Y,N));
z = y(1:en-start+1);
sig_out(start:en)=z;
end;
y = sig_out*5000 % Multiplying 5000 is just increasing the intensity of the o/p signal

% --- Executes on button press in pushbutton9.
function pushbutton9_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global Fs;
global f;
global fp;
% fa=f*2;
Psat=100;
fp = powerCompress(f, Psat,Fs);
sound(fp,Fs);
axes(handles.axes8);
N2=length(fp);
t=(0:N2-1)/Fs;
plot(t,fp);
title('Amplitued-Adjused Signal');


% --- Executes on button press in fourier3.
function fourier3_Callback(hObject, eventdata, handles)
% hObject    handle to fourier3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global y;
global Fs;
c = fft(y);     % for plot the noise contained signal in frequency              
nfft = length(c);
c = c(1:nfft/2);
P2 = abs(c);
xfft = Fs*(0:nfft/2-1)/nfft;
axes(handles.axes6);
plot(xfft,P2,'r');          % end of plot the freq: domain signal
title('Denoise signal in frequency domain')
guidata(hObject,handles);


% --- Executes on button press in Back.
function Back_Callback(hObject, eventdata, handles)
% hObject    handle to Back (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
cover;
closereq;


% --- Executes on button press in fourier4.
function fourier4_Callback(hObject, eventdata, handles)
% hObject    handle to fourier4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global f;
global Fs;
c = fft(f);     % for plot the noise contained signal in frequency              
nfft = length(c);
c = c(1:nfft/2);
P2 = abs(c);
xfft = Fs*(0:nfft/2-1)/nfft;
axes(handles.axes12);
plot(xfft,P2,'r');          % end of plot the freq: domain signal
title('Frequency shaped signal in frequency domain')
guidata(hObject,handles);


% --- Executes on button press in fourier5.
function fourier5_Callback(hObject, eventdata, handles)
% hObject    handle to fourier5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global fp;
global Fs;
c = fft(fp);     % for plot the noise contained signal in frequency              
nfft = length(c);
c = c(1:nfft/2);
P2 = abs(c);
xfft = Fs*(0:nfft/2-1)/nfft;
axes(handles.axes13);
plot(xfft,P2,'r');          % end of plot the freq: domain signal
title('Compressed signal in frequency domain')
guidata(hObject,handles);
