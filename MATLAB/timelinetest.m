data_with_init_time = [ 
       1, 10, 5, 3 ;
       3, 10, 3, 9 ;
       7, 10, 4, 8 ;
       12,10, 2, 2 ];

h = barh(data_with_init_time, 'stack');
set(h(1), 'facecolor', 'none', 'EdgeColor', 'none'); % disable the color of the first column (init time)
set(gca, 'YTickLabel', {'proc 1', 'proc 2', 'proc 3', 'proc 4'} ); % change the y axis tick to your name of the process
axis ij; % Put the first row at top