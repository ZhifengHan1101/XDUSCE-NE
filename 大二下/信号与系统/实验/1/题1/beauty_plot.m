function beautiplot(model)
% --input : model
% --model : 'default'
%         : 'small'
% --usage :
%         beautiplot
%         beautiplot('small')
%%
if nargin<1
	model = 'default';
end
fig=gcf;
% fontname = 'Times New Roman';
fontname = 'STFangSong';
%% set Figure
set(fig,'Units','pixels');
%%
figchildren = fig.Children;
for ax_i = 1 : length(figchildren)
    figChildren =  figchildren(ax_i);
    if strcmp(figChildren.Type,'axes') == 1
        ax = figChildren;
        class1=ax.Children;
        
        %% set Axes
        % 字体
        set(ax,'FontName',fontname,'FontSize',10,'FontWeight','bold');
        % 网格
        set(ax,'XGrid','on','YGrid','on','GridLineStyle','--','GridAlpha',0.15);
        % 边框
        set(ax,'Box','off','LineWidth',1.5);
        % 刻度
        set(ax,'XMinorTick','on','YMinorTick','on');
        % 标题
        
        switch model
            case 'default'
                set(ax.XLabel,'FontSize',12,'FontWeight', 'bold');%normal
                set(ax.YLabel,'FontSize',12,'FontWeight', 'bold');
                set(ax.ZLabel,'FontSize',12,'FontWeight', 'bold');
                set(ax.Title,'FontSize',13,'FontWeight', 'bold');
            case 'small'
                set(ax.XLabel,'FontSize',10,'FontWeight', 'bold');%normal
                set(ax.YLabel,'FontSize',10,'FontWeight', 'bold');
                set(ax.ZLabel,'FontSize',10,'FontWeight', 'bold');
                set(ax.Title,'FontSize',11,'FontWeight', 'bold');
        end
        %% set Legend
%         try
%             set(ax.Legend,'Location','best','FontName',fontname,...%'Orientation','horizontal'
%             'FontWeight', 'bold');
%         catch
        try
            set(ax.Legend,'FontName',fontname,...%'Orientation','horizontal'
            'FontWeight', 'bold');
        catch
            
        end
        %% set class
        for i=1:length(class1)
            % line(线图)
            if strcmp(class1(i).Type,'line')==1
                set(class1(i),'LineWidth',2,'MarkerSize',6);
                % scatter(散点图)
            elseif strcmp(class1(i).Type,'scatter')==1
                set(class1(i),'MarkerFaceColor',[0.15,0.15,0.15],...
                    'LineWidth',2,...
                    'MarkerFaceAlpha',0.8);
                % bar(柱状图)
%             elseif strcmp(class1(i).Type,'bar')==1
%                 set(class1(i),'FaceColor',[0.47,0.67,0.19],'EdgeColor',[0 0 0],...
%                     'LineWidth',1.5);
            elseif strcmp(class1(i).Type,'bar')==1
                set(class1(i),...
                    'LineWidth',1.5);
                % text(文字)
            elseif strcmp(class1(i).Type,'text')==1
                set(class1(i),'FontSize',12,'FontName',fontname,'FontWeight', 'bold');
            end
        end
    end
end

end