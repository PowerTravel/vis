function [ y ] = getbb( )
%MAPBB Summary of this function goes here
%   Detailed explanation goes here


    bb = load('boundingBoxes.m');
    bb = bb';

    y = zeros(8,3);
    
    y( 1, 1:3) = bb(1:3,1);
    y( 2, 1:3) = bb(1:3,2);
    y( 3, 1:3) = bb(1:3,3);
    y( 4, 1:3) = bb(1:3,4);

    y( 5, 1:3) = bb(1:3,5);
    y( 6, 1:3) = bb(1:3,6);
    y( 7, 1:3) = bb(1:3,7);
    y( 8, 1:3) = bb(1:3,8);

    y=y';
    
end

