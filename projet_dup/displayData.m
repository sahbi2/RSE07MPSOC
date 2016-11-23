close ;
data = dlmread('data.txt');
centroids = dlmread('centroids.txt');

if(size(data, 2) == 2)
    scatter(data(:, 1), data(:, 2));
    hold on ;
    scatter(centroids(:, 1), centroids(:, 2), 'MarkerFaceColor', 'red')
elseif(size(data, 2) == 3)
    scatter3(data(:, 1), data(:, 2), data(:, 3));
    hold on ;
    scatter3(centroids(:, 1), centroids(:, 2), centroids(:, 3), 'MarkerFaceColor', 'red')    
else
    echo "Data must in 2D or in 3D"
end
  