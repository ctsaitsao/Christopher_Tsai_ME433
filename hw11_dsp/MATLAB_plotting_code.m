clear all;
close all;
clc;

A = dlmread('data.txt');
t = A(:, 1);
raw = A(:, 2);
MAF = A(:, 3);
IIR = A(:, 4);
FIR = A(:, 5);
figure
hold on
plot(t, raw)
plot(t, MAF)
plot(t, IIR)
plot(t, FIR)
xlabel('time')
ylabel('accelZ_scaled')
legend('raw','MAF','IIR','FIR')