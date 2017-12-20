format compact 
h(1) = axes('Position',[0.2 0.2 0.6 0.6]);
%---- Bounding Boxes ------
filename = fullfile('D:\','3rd_sem','AdvancedProject', 'Visualization', 'boundingBoxSTR.dat');
%disp(filename)
fileID = fopen(filename);
for i = 1:4
    C = textscan(fileID,'2465STR%f %f32 %f32 %f32 %f32 %f32 %f32', 1);
    %celldisp(C);
    x1 = C{2};
    y1 = C{3};
    z1 = C{4};
    x2 = C{5};
    y2 = C{6};
    z2 = C{7};
    vert = [x1 y1 z1; x1 y2 z1; x2 y2 z1; x2 y1 z1 ; ...
        x1 y1 z2; x1 y2 z2; x2 y2 z2; x2 y1 z2];
    fac = [1 2 3 4; ...
        2 6 7 3; ...
        4 3 7 8; ...
        1 5 8 4; ...
        1 2 6 5; ...
        5 6 7 8];
    patch('Faces',fac,'Vertices',vert,'FaceColor','r');  % patch function
    material shiny;
    alpha('color');
    alphamap('rampdown');
    view(10,5);
end
fclose(fileID);

%---- Spatial Objects ------
filename = fullfile('D:\','3rd_sem','AdvancedProject', 'Visualization', 'testObj.dat');
disp(filename)
fileID = fopen(filename);
for i = 1:10
    C = textscan(fileID,'2465 %f32 %f32 %f32 %f32 %f32 %f32', 1);
    %celldisp(C);
    x1 = C{1};
    y1 = C{2};
    z1 = C{3};
    x2 = C{4};
    y2 = C{5};
    z2 = C{6};
    vert = [x1 y1 z1; x1 y2 z1; x2 y2 z1; x2 y1 z1 ; ...
        x1 y1 z2; x1 y2 z2; x2 y2 z2; x2 y1 z2];
    fac = [1 2 3 4; ...
        2 6 7 3; ...
        4 3 7 8; ...
        1 5 8 4; ...
        1 2 6 5; ...
        5 6 7 8];
    patch('Faces',fac,'Vertices',vert,'FaceColor','b');  % patch function
    material shiny;
    alpha('color');
    alphamap('rampdown');
    view(10,5);
end
fclose(fileID);