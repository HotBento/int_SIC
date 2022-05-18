%%
higgs = fileread("v0\higgs-activity_time2022_5_5_17_3.txt");
ubuntu = fileread("v0\sx-askubuntu2022_4_29_14_4.txt");
stack = fileread("v0\sx-stackoverflow2022_5_2_13_12.txt");
super = fileread("v0\sx-superuser2022_5_4_11_18.txt");

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
v0_higgs_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
v0_higgs_inf = sum/cnt;
%%
throughput = regexp(ubuntu, throughput_expr, 'match');
influence = regexp(ubuntu, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
v0_ubuntu_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
v0_ubuntu_inf = sum/cnt;
%%
throughput = regexp(stack, throughput_expr, 'match');
influence = regexp(stack, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
v0_stack_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
v0_stack_inf = sum/cnt;
%%
throughput = regexp(super, throughput_expr, 'match');
influence = regexp(super, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
v0_super_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
v0_super_inf = sum/cnt;
%%
clearvars -except *inf *thr