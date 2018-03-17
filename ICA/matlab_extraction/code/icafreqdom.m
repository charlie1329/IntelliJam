function [out] = icafreqdom(input_path,output_prefix) 
import stft
% function carries out blind source separation in the frequency domain
% input_path is the file to read from (should be stereo)
% output_prefix is the first part of the output file
% since there are multiple files, these have to be indexed.
% Author: Charlie Street

% first read in the data
[X, fs] = audioread(input_path);

XMono = (X(:,1) + X(:,2))/2; % average across both channels

disp(size(XMono));

% do a short time fourier transform
Xlen = length(XMono);
t = (0:Xlen-1)/fs;
wlen = 1024;
hop = wlen/4;
nfft = 10 * wlen;

[stftv, f, t] = stft(XMono, wlen, hop, nfft, fs);
disp('done stft');
disp(size(stftv));
% split into absolute values and phase information
X = arrayfun(@(x) real(x),stftv);
Phi = arrayfun(@(x) imag(x),stftv);

disp('About to do Eigen Vectors');
% project the data into a lower dimensional subspace
[V,D] = eig(cov(X')); %get eigenvectors/values

disp('Done Eigen Vectors');
[D,I] = sort(diag(D),'descend');

D = diag(D); % eigenvalues
V = V(:,I); % eigenvectors

disp(size(V));

%do PCA
totalVariance = sum(diag(D));
currentPreserved = 0;
dim = 0;

while (currentPreserved/totalVariance) < 0.9
    dim = dim + 1;
    currentPreserved = currentPreserved + D(dim,dim);
end

disp('Dimensions to keep:');
disp(dim);

DPrime = D(1:dim,:); % get best rows

T = DPrime * V';

disp(size(T));

XPrime = T * X;

disp(size(XPrime));
disp('about to start ICA');
%run ica
[E, H, iter, W] = robustica(XPrime,{});

disp('Done ICA');
disp(size(E));

% start trying to reconstruct
F = pinv(pinv(H) * T); 

disp('Created F'); 
disp(size(F));

% now create reconstructed things
% on a bigger system this could be done in parallel
disp('Creating reconstructed spectrograms');
for c = 1:dim
   S = F(:,c) * E(c,:);
   disp(size(S));
   disp('Add back phase information');
   numElems = length(S(:));
   for elem = 1:numElems
      S(elem) = S(elem) * complex(cos(Phi(elem)),sin(Phi(elem)));
   end
   
   disp('Do inverse stft');
   [source, t] = istft(S,wlen,hop,nfft,fs);
   disp(size(source));
   
   disp('Writing source to file');
   current_file_out = strcat(output_prefix,strcat(int2str(c),'.wav'));
   audiowrite(current_file_out,source,fs);
   disp('Finished writing');
end


