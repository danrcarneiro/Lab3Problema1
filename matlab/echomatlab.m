infile='entrada.wav';
outfile='saida.wav';
% read the sample waveform
[x,Fs] = audioread(infile);
% normalize x to +/- 1 amplitude
x = x ./ (max(abs(x)));
% parameters 
N_delay=44100/2; % delay in samples
C = 0.6; % amplitude of direct sound
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
y = zeros(length(x),1);       % create empty out vector
y(1:N_delay)=x(1:N_delay); % to avoid referencing of negative samples
% for each sample > N_delay
for i = (N_delay+1):length(x)
    y(i) = C*x(i) + (1-C)*(x(i-N_delay));   % add delayed sample
    
end
% write output
% normalize y to +/- 1 amplitude
y = y ./ (max(abs(y)));
audiowrite(outfile, y, Fs);
figure(1)
hold on
plot(x,'r');
plot(y,'b');
title('Echoed and original Signal');
%sound(y,Fs);