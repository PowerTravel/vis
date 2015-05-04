function [ x ] = getBox( p , gs )
%GETBOX Summary of this function goes here
%   Detailed explanation goes here

    x = zeros(3,8);
    
    p = p*gs;
    
    x(1,1) = p(1);
    x(1,2) = p(1) + gs;
    x(1,3) = p(1) + gs;
    x(1,4) = p(1);
    x(1,5) = p(1);
    x(1,6) = p(1) + gs;
    x(1,7) = p(1) + gs;
    x(1,8) = p(1);
    
    x(2,1) = p(2);
    x(2,2) = p(2);
    x(2,3) = p(2) + gs;
    x(2,4) = p(2) + gs;
    x(2,5) = p(2);
    x(2,6) = p(2);
    x(2,7) = p(2) + gs;
    x(2,8) = p(2) + gs;
    
    x(3,1) = p(3);
    x(3,2) = p(3);
    x(3,3) = p(3);
    x(3,4) = p(3);
    x(3,5) = p(3) + gs;
    x(3,6) = p(3) + gs;
    x(3,7) = p(3) + gs;
    x(3,8) = p(3) + gs;

end

