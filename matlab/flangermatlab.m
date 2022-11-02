clear
close all
infile='../assets/music.wav';
outfile='../assets/saidaMatlabFlangerFloat.wav';
outfileFP = '../assets/saidaMatlabFlangerFixed.wav';
[x,Fs] = audioread(infile);
x = x(1:2000000);
fs=Fs;

vmix = 0.75;
vdelay = 5;
vwidth = 5;
vrate = 0.7;

y = flanger(x,vmix,vdelay,vwidth,vrate,Fs);

%MIX = 0.75

audiowrite(outfile, y, Fs);
figure(1)
hold on
plot(y,'r');
plot(x,'b');
title('Sinal original vs com flanger');
xlabel("Amostras")
ylabel("Amplitude")


%% Fixed point

x_fixed = fi(x);
x_converted = double(x);
[y_fixed, LFO] = flangerFixed(x,vmix,vdelay,vwidth,vrate,Fs);

figure(2)
hold on
plot(y,'r');
plot(y_fixed,'b');
title('Sinal em double vs ponto fixo');
%sound(y,Fs);

err = immse(y_fixed,y)


%% Function


function [out] = flanger(in,mix,delay,width,rate,fs)
%FLANGER simulates a guitar flanger effect pedal
%   IN - input vector
%   MIX - depth - amt of delayed signal added to IN (0 to 1)
%   DELAY - min delay time - 100usec to 10msec (in msec) 0.1 to 10
%   WIDTH - sweep depth - how wide sweep is (100nsec to 10msec) 
%           (in msec, 0.0001)
%   RATE - frequency of LFO - 0.05 to 5 Hz

in=interp1(1:length(in),in,1:.25:length(in));
fsn=fs*4;

minDelaySamp=ceil(delay*fsn/1000); %convert to msec, then samples
maxDelaySamp=ceil((delay+width)*fsn/1000); %convert to msec, then samples
n=(1:length(in)+maxDelaySamp)'; %how long to extend in by for LFO
LFO=sawtooth(2*pi*rate/(fsn)*n,.5); %sawtooth more commonly used in flangers
delayTimeSamples=(delay+width/2+width/2*LFO)*fsn/1000; 
% instantaneous delay in samples (computed by looking at graph from class
% PDF)

out=zeros(length(in)+minDelaySamp,1); %initialized output vec

out(1:maxDelaySamp)=in(1:maxDelaySamp); 
% copy front of signal before min delay

for i=maxDelaySamp+1:length(in) % starting from next sample
    delaySamples=ceil(delayTimeSamples(i)); %whole number of current delay
    out(i)=in(i)+mix*out(i-delaySamples); %add input and fraction of delay 
end

out=downsample(out,4);

end

function [out,LFO] = flangerFixed(in,mix,delay,width,rate,fs)
%FLANGER simulates a guitar flanger effect pedal
%   IN - input vector
%   MIX - depth - amt of delayed signal added to IN (0 to 1)
%   DELAY - min delay time - 100usec to 10msec (in msec) 0.1 to 10
%   WIDTH - sweep depth - how wide sweep is (100nsec to 10msec) 
%           (in msec, 0.0001)
%   RATE - frequency of LFO - 0.05 to 5 Hz

in=interp1(1:length(in),in,1:.25:length(in));
fsn=fs*4;

minDelaySamp=ceil(delay*fsn/1000); %convert to msec, then samples
maxDelaySamp=ceil((delay+width)*fsn/1000); %convert to msec, then samples
n=(1:length(in)+maxDelaySamp)'; %how long to extend in by for LFO
LFO=double(fi(sawtooth(2*pi*rate/(fsn)*n,.5))); %sawtooth more commonly used in flangers
delayTimeSamples=(delay+width/2+width/2*LFO)*fsn/1000; 
% instantaneous delay in samples (computed by looking at graph from class
% PDF)

out=zeros(length(in)+minDelaySamp,1); %initialized output vec

out(1:maxDelaySamp)=in(1:maxDelaySamp); 
% copy front of signal before min delay

for i=maxDelaySamp+1:length(in) % starting from next sample
    delaySamples=ceil(delayTimeSamples(i)); %whole number of current delay
    out(i)=in(i)+mix*out(i-delaySamples); %add input and fraction of delay 
end

out=double(fi(downsample(out,4)));

end
