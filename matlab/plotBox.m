function [ y ] = plotBox( x )
%PLOTBOX Summary of this function goes here
%   Detailed explanation goes here

a = zeros(3,5);
a(:,1:4) = x(:,1:4);
a(:,5) = x(:,1);

b = zeros(3,5);
b(:,1:4) = x(:,5:8);
b(:,5) = x(:,5);

plot3(a(1,:), a(2,:), a(3,:));
hold on
plot3(b(1,:), b(2,:), b(3,:));


for i = [1,2,3,4]
    l = [a(:,i) , b(:,i)];
    plot3(l(1,:),l(2,:),l(3,:))
end
y=0;
end

