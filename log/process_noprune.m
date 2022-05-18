%%
higgs = fileread("loop_noseedprune\higgs-activity_time2022_5_5_17_22.txt");
ubuntu = fileread("loop_noseedprune\sx-askubuntu2022_4_28_12_52.txt");
stack = fileread("loop_noseedprune\sx-stackoverflow2022_4_30_13_43.txt");
super = fileread("loop_noseedprune\sx-superuser2022_5_3_14_27.txt");

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
noprune_higgs_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
noprune_higgs_inf = sum/cnt;
%%
throughput = regexp(ubuntu, throughput_expr, 'match');
influence = regexp(ubuntu, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
noprune_ubuntu_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
noprune_ubuntu_inf = sum/cnt;
%%
throughput = regexp(stack, throughput_expr, 'match');
influence = regexp(stack, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
noprune_stack_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
noprune_stack_inf = sum/cnt;
%%
throughput = regexp(super, throughput_expr, 'match');
influence = regexp(super, influence_expr, 'match');
cnt = 0;
sum = 0;
for i = throughput
    sum = sum + str2double(i{1}(12:end-2));
    cnt = cnt + 1;
end
noprune_super_thr = sum/cnt/2500;
cnt = 0;
sum = 0;
for i = influence
    sum = sum + str2double(i{1}(18:end));
    cnt = cnt + 1;
end
noprune_super_inf = sum/cnt;
%%
clearvars -except *inf *thr