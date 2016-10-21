function [] = MakeGraph()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

dataTableBisection = readtable('bisection.csv');
dataTableNR1 = readtable('newtonraphson1.csv');
dataTableNR2 = readtable('newtonraphson2.csv');

assignin('base', 'table1', dataTableBisection);
assignin('base', 'table2', dataTableNR1);
assignin('base', 'table3', dataTableNR2);

finalResult = dataTableBisection{end,end};

plot(dataTableBisection{:,1},dataTableBisection{:,2},dataTableNR1{:,1}, dataTableNR1{:,3}-finalResult,dataTableNR2{:,1}, dataTableNR2{:,3}-finalResult);
xlabel('Iteration');
ylabel('Absolute error');
legend('Bisection Method', 'Newton-Raphson method 1', 'Newton-Raphson method 2');

end

