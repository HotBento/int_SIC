%%
% get data
process_loop;
process_noprune;
process_v0;
process_recurse;
%%
% experiment 1
f1 = figure(1);
x = categorical({'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
x = reordercats(x,{'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
y = [v0_higgs_thr, loop_higgs_thr; v0_ubuntu_thr, loop_ubuntu_thr; v0_stack_thr, loop_stack_thr; v0_super_thr, loop_super_thr];
b = bar(x, y, 1);
ylim([0 350]);
xtips1 = b(1).XEndPoints;
ytips1 = b(1).YEndPoints;
labels1 = string(roundn(b(1).YData,-1));
text(xtips1,ytips1,labels1,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
xtips2 = b(2).XEndPoints;
ytips2 = b(2).YEndPoints;
labels2 = string(roundn(b(2).YData,-1));
text(xtips2,ytips2,labels2,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
legend('unoptimized', 'optimized');
ylabel('处理每次交互需要的时间（ms）')
set(f1, 'position', [100 100 1000 300]);
%%
f2 = figure(2)
x = categorical({'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
x = reordercats(x,{'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
y = [v0_higgs_inf, loop_higgs_inf; v0_ubuntu_inf, loop_ubuntu_inf; v0_stack_inf, loop_stack_inf; v0_super_inf, loop_super_inf];
b = bar(x, y, 1);
xtips1 = b(1).XEndPoints;
ytips1 = b(1).YEndPoints;
labels1 = string(roundn(b(1).YData,-1));
text(xtips1,ytips1,labels1,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
xtips2 = b(2).XEndPoints;
ytips2 = b(2).YEndPoints;
labels2 = string(roundn(b(2).YData,-1));
text(xtips2,ytips2,labels2,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
legend('unoptimized', 'optimized');
ylabel('影响力传播范围')
set(f2, 'position', [100 100 1000 300]);
%%
f3 = figure(3)
x = categorical({'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
x = reordercats(x,{'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
y = [noprune_higgs_thr, loop_higgs_thr; noprune_ubuntu_thr, loop_ubuntu_thr; noprune_stack_thr, loop_stack_thr; noprune_super_thr, loop_super_thr];
b = bar(x, y, 1);
ylim([0 100]);
xtips1 = b(1).XEndPoints;
ytips1 = b(1).YEndPoints;
labels1 = string(roundn(b(1).YData,-1));
text(xtips1,ytips1,labels1,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
xtips2 = b(2).XEndPoints;
ytips2 = b(2).YEndPoints;
labels2 = string(roundn(b(2).YData,-1));
text(xtips2,ytips2,labels2,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
legend('unoptimized', 'optimized');
ylabel('处理每次交互需要的时间（ms）')
set(f3, 'position', [100 100 1000 300]);
%%
f4 = figure(4)
x = categorical({'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
x = reordercats(x,{'higgs-twitter', 'sx-askubuntu', 'sx-stackoverflow', 'sx-superuser'});
y = [noprune_higgs_inf, loop_higgs_inf; noprune_ubuntu_inf, loop_ubuntu_inf; noprune_stack_inf, loop_stack_inf; noprune_super_inf, loop_super_inf];
bar(y)
b = bar(x, y, 1);
xtips1 = b(1).XEndPoints;
ytips1 = b(1).YEndPoints;
labels1 = string(roundn(b(1).YData,-1));
text(xtips1,ytips1,labels1,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
xtips2 = b(2).XEndPoints;
ytips2 = b(2).YEndPoints;
labels2 = string(roundn(b(2).YData,-1));
text(xtips2,ytips2,labels2,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom', 'FontSize',6)
legend('unoptimized', 'optimized');
ylabel('影响力传播范围')
ylim([1000 5000]);
set(f4, 'position', [100 100 1000 300]);