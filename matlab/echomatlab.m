clear
infile='../assets/music.wav';
outfile='../assets/saidaMatlabEchoFloat.wav';
outfileFP = '../assets/saidaMatlabEchoFixed.wav';
[x,Fs] = audioread(infile); %

until = 44100*10;

%% Double precision

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
title('Sinal original vs com echo');
xlabel("Amostras")
ylabel("Amplitude")
%sound(y,Fs);

%% Fixed point
x_fixed = fi(x);
x_fixedConv = double(x_fixed);
y_fixed = zeros(length(double(x)),1);       % create empty out vector
y_fixed(1:N_delay)=double(x(1:N_delay)); % to avoid referencing of negative samples
% for each sample > N_delay
for i = (N_delay+1):length(double(x_fixedConv))
    y_fixed(i) = C*double(x_fixedConv(i)) + (1-C)*(double(x_fixedConv(i-N_delay)));   % add delayed sample
    fprintf("%.2f%%\n", 100*i/length(x_fixedConv));
end
% write output
% normalize y to +/- 1 amplitude
y_fixed = y_fixed ./ (max(abs(y_fixed)));
y_fixedObj = fi(y_fixed);
y_fixedConverted = double(y_fixedObj);
audiowrite(outfileFP, double(y_fixedObj), Fs);
figure(2)
hold on
plot(y,'r');
plot(y_fixedConverted,'b');
title('Sinal original e ponto fixo - Fixed point');
%sound(y,Fs);

err = immse(y_fixedConverted,y)