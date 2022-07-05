clear;
close all;

%% 시리얼 통신 연결부분 
fprintf("******  Incoming Data from Arduino *****\n");
device = serialport('COM11', 115200)
configureTerminator(device, "CR/LF");
%% 그래프 설정 
figure(1);
hold on;
grid on;
ylim([-15 15])



%% 그래프 애니메이션 부분
 for i = 1:10000
     if read(device, 1, 'uint8') == 2
        
        readdata = readline(device);
        data = split(readdata);

        plotData = str2double(data)
        plot(i,plotData(1),'.')
        plot(i,plotData(2),'.')
        plot(i,plotData(3),'.')
        drawnow;
     else
         flush(device)
     end

end
