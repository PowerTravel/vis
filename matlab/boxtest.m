clear
hold off
gridPoint = load('gridBoxes.m');

gridPoint = gridPoint';

bb = getbb();

plot3(bb(1,:),bb(2,:),bb(3,:), '-m');

gridsize = 10;
p = size(gridPoint(1,:));
p = p(2);

x = zeros(3, 8*p);
hold on

for i=[0:p-1]
    x(:, 8*i+1 : 8*i+8) = getBox(gridPoint(:,i+1), gridsize);
    A  =  getBox(gridPoint(:,i+1), gridsize);
    plotBox(A);
  %  pause(1)
end
axis equal