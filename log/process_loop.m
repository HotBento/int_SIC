%%
higgs = fileread("loop\higgs-activity_time2022_5_5_17_18.txt");
ubuntu = fileread("loop\sx-askubuntu2022_4_27_19_45.txt");
stack = fileread("loop\sx-stackoverflow2022_4_30_11_25.txt");
super = fileread("loop\sx-superuser2022_5_3_13_24.txt");

throughput_expr = "Time cost: [^\n]*";
influence_expr = "Influence spread: [^\n]*";
%%
throughput = regexp(higgs, throughput_expr, 'match');
influence = regexp(higgs, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
loop_higgs_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
loop_higgs_inf = sum/cnt;
%%
throughput = regexp(ubuntu, throughput_expr, 'match');
influence = regexp(ubuntu, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
loop_ubuntu_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
loop_ubuntu_inf = sum/cnt;
%%
throughput = regexp(stack, throughput_expr, 'match');
influence = regexp(stack, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
loop_stack_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
loop_stack_inf = sum/cnt;
%%
throughput = regexp(super, throughput_expr, 'match');
influence = regexp(super, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
loop_super_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
loop_super_inf = sum/cnt;
%%
clearvars -except *inf *thr