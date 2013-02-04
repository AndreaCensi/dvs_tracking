mydata=rand(20,2)
index = 1:20;

bar_h=barh(mydata,'stack');
bar_child=get(bar_h(2),'Children');
set(bar_child, 'CData',index);