clear;
close all;

%% 시리얼 통신 연결부분 
fprintf("******  Incoming Data from Arduino *****\n");
device = serialport('COM11', 115200)
configureTerminator(device, "CR/LF");

%% 도형 그래픽 부분
cube = [-5  5 -6; % 1
         5  5 -6; % 2
         5 -5 -6; % 3
        -5 -5 -6; % 4
        -5  5  6; % 5
         5  5  6; % 6
         5 -5  6; % 7 
        -5 -5  6; % 8
       ];

zbar = [-1  1  6;
         1  1  6;
         1 -1  6;
        -1 -1  6;
        -1  1  18;
         1  1  18;
         1 -1  18;
        -1 -1  18;
       ];

xbar = [ 5  -1  1;
         5   1  1;
         5   1 -1;
         5  -1 -1;
         15 -1  1;
         15  1  1;
         15  1 -1;
         15 -1 -1;
       ];

ybar = [ 1  15  1;
        -1  15  1;
        -1  15 -1;
         1  15 -1;
         1   5  1;
        -1   5  1;
        -1   5 -1;
         1   5 -1;
       ];

allface = [ 1 2 3 4;
            5 6 7 8;
            2 3 7 6;
            3 4 8 7;
            1 4 8 5;
            1 2 6 5;
          ];

face = [1 2 3 4;
        2 3 7 6;
        3 4 8 7;
        1 4 8 5;
        1 2 6 5;];

cap = [5 6 7 8];

%% 그래프 설정 
fig = figure(1);
fig_axis = axes('Parent',fig);
axis([-15 15 -15 15 -15 15])
%xlim([-20 20])
%ylim([-20 20])
%zlim([-20 20])
xlabel('X');
ylabel('Y');
zlabel('Z');
view(3)
grid on;
set(fig_axis,'XDir','reverse','YDir','reverse');


%% 그래프 애니메이션 부분
 for i = 1:10000


    if read(device,1, 'uint8') == 2

        double_ypr = 0;
        readdata = readline(device);
        ypr = split(readdata);
        double_ypr = str2double(ypr)

        % 회전각도 설정
        turn_x_ang = double_ypr(1);
        turn_y_ang = double_ypr(2);
        z_ang = double_ypr(3);

        fprintf("%f.2, %f.2, %f.2\n", double_ypr(1), double_ypr(2), double_ypr(3))

    
        tic
        cla
        s = patch('vertices',cube,'faces',face,'Facecolor',[0.8 1 0.8]);
        top = patch('vertices', cube, 'faces',cap,'Facecolor',[1 0.8 0.8]);
        z_bar = patch('vertices',zbar, 'faces', allface,'Facecolor','red');
        x_bar = patch('vertices',xbar, 'faces', allface,'Facecolor','green');
        y_bar = patch('vertices',ybar, 'faces', allface,'Facecolor','blue');
    

        rotate(s,[1 0 0], turn_x_ang)
        rotate(top,[1 0 0], turn_x_ang)
        rotate(z_bar, [1 0 0], turn_x_ang)
        rotate(x_bar, [1 0 0], turn_x_ang)
        rotate(y_bar, [1 0 0], turn_x_ang)

        rotate(s, [0 1 0], turn_y_ang)
        rotate(top, [0 1 0], turn_y_ang)
        rotate(z_bar, [0 1 0], turn_y_ang)
        rotate(x_bar, [0 1 0], turn_y_ang)
        rotate(y_bar, [0 1 0], turn_y_ang)

        rotate(s, [0 0 1], z_ang)
        rotate(top, [0 0 1], z_ang)
        rotate(z_bar, [0 0 1], z_ang)
        rotate(x_bar, [0 0 1], z_ang)
        rotate(y_bar, [0 0 1], z_ang)


        drawnow;
        flush(device,"input")

    else
        % 시작바이트 2가 인식되지 않으면 버퍼 비우기
        flush(device)
    end
    
    % 연산시간 측정 및 애니메이션 균일화
    % 그래프 업데이트 주기를 균일화 하기 위해 0.03초 고정으로 그래프가 
    % 업데이트 되도록 pause() 추가
    %t = toc;
    %pausetime = 0.03-t;
    %if pausetime < 0
    %    pausetime = 0;
    %    pause(pausetime);
    %else
    %    pause(pausetime)
    %enda
    %pause(0.01)
end
