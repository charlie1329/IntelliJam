function [out] = wavToGraph(input_path, name)
%wavToGraph Finds distribution of a sound file
%   input_path is the file to read from
%   name is the title of the graph output from the function
%   i assume mono tracks, so I will just take the left channel
%   all of my samples are centred tracks anyway, so this is fine.
% Author: Charlie Street

[samples, sampleRate] = audioread(input_path);

mono = samples(:,1);

xbars = linspace(-1,1,201); %-1 to 1 in 0.01 increments

buckets = zeros(1,201);

numSamples = size(mono,1);

for i = 1:numSamples
    if mono(i,1) < -1
        currentBucket = 1;
    elseif mono(i,1) > 1
        currentBucket = 201;
    else
        currentBucket = floor(((mono(i,1) + 1)/2) * 201) + 1;
    end
    buckets(1,currentBucket) = buckets(1,currentBucket) + 1;
end

bar(xbars,buckets/numSamples);
title(name);
xlabel('Sample Value (16 bit PCM)');
xlim([-1.0 1.0]);
ylabel('Relative Frequency');
end

