function [out] = blues(input_data, output_prefix, sample_rate, depth)

% function carries out blind underdetermined 
% source separation from music
% taken from the BLUES research paper
% Author: Charlie Street

% carry out ica
S = robustica(input_data, {});

% retrieve each component
% scaling them to have unit variance
disp('Done ICA');
y1 = S(1,:)/std(S(1,:));

y2 = S(2,:)/std(S(2,:));

window_length = 512;
fft_length = 2048;
hop_size = 128; %75 percent overlap as in paper

%convert into T-F domain
[y1stft, ~, ~] = stft(y1,window_length,hop_size,fft_length,sample_rate);
[y2stft, ~, ~] = stft(y2,window_length,hop_size,fft_length,sample_rate);

disp('Done fourier transform of sources');

tau = 1.5;

%create binary masks
BM1 = (tau * abs(y1stft)) > abs(y2stft);
BM2 = (tau * abs(y2stft)) > abs(y1stft);

disp('Created masks');

% convert mixtures into TF domain
[x1, ~, ~] = stft(input_data(1,:),window_length,hop_size,fft_length,sample_rate);
[x2, ~, ~] = stft(input_data(2,:),window_length,hop_size,fft_length,sample_rate);

disp('Brought sources into TF domain');

x1bm1 = BM1.* x1;
x2bm1 = BM1.* x2;

x1bm2 = BM2.* x1;
x2bm2 = BM2.* x2;

disp('Applied the masks');

% generate masked output signals
x11 = istft(x1bm1,window_length,hop_size,fft_length,sample_rate);
x21 = istft(x2bm1,window_length,hop_size,fft_length,sample_rate);

X1 = [x11; x21];

x21 = istft(x1bm2,window_length,hop_size,fft_length,sample_rate);
x22 = istft(x2bm2,window_length,hop_size,fft_length,sample_rate);

X2 = [x21; x22];

disp('Generated masked output signals');

% now do some checks as to whether we should stop or continue running

X1Cov = cov(X1');
% should be 2x2
Rx1 = cond(X1Cov);
disp(Rx1);
X2Cov = cov(X2');
Rx2 = cond(X2Cov);
disp(Rx2);
condThreshold = 250;
maxDepth = 10;

keepGoing1 = false;
keepGoing2 = false;

if Rx1 >= condThreshold || depth == maxDepth
    filename = strcat(output_prefix,strcat(int2str(depth),'1.wav'));
    audiowrite(filename,X1',sample_rate);
    disp('Written a file out');
else
    keepGoing1 = true;
end

if Rx2 >= condThreshold || depth == maxDepth
    filename = strcat(output_prefix,strcat(int2str(depth),'2.wav'));
    audiowrite(filename,X2',sample_rate);
    disp('Written a file out');
else
    keepGoing2 = true;
end

if keepGoing1 && keepGoing2
    if Rx1 > Rx2
        keepGoing2 = false;
    else
        keepGoing1 = false;
    end
end

if keepGoing1 && depth < maxDepth
    disp('Recursive call 1');
    blues(X1,output_prefix,sample_rate,depth+1);
end

if keepGoing2 && depth < maxDepth
    disp('Recursive call 2');
    blues(X2,output_prefix,sample_rate,depth+1);
end

disp('Finished');
