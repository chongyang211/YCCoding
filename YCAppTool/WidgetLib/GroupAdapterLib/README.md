# YCGroupAdapter
- 01.前沿说明
    - 1.1 案例展示效果
    - 1.2 该库功能和优势
    - 1.3 相关类介绍说明
- 02.如何使用
    - 2.1 如何引入
    - 2.2 最简单使用
    - 2.3 使用建议
- 03.常用api
    - 3.1 自定义adapter
    - 3.2 notify相关
    - 3.3 点击事件listener
- 04.实现步骤
    - 4.1 业务需求分析
    - 4.2 adapter实现多type
    - 4.3 这样写的弊端
    - 4.4 分组实体bean
    - 4.5 构建封装adapter
- 05.优化相关
- 06.关于参考
- 07.其他说明介绍



### 01.前沿说明
#### 1.1 案例展示效果
- demo中的效果图
- ![image](https://img-blog.csdnimg.cn/20200216114835550.png)
![image](https://img-blog.csdnimg.cn/20200216114902608.png)
![image](https://img-blog.csdnimg.cn/20200216115001459.png)
![image](https://img-blog.csdnimg.cn/20200216115025781.png)
![image](https://img-blog.csdnimg.cn/20200216115049203.png)
![image](https://img-blog.csdnimg.cn/20200216115111852.png)
![image](https://img-blog.csdnimg.cn/20200216115135178.png)


- 实际项目中的效果图
- ![image](https://img-blog.csdnimg.cn/20200216115157836.png)
![image](https://img-blog.csdnimg.cn/20200216115220512.png)




#### 1.2 该库功能和优势
- 按组划分的自定义adapter适配器，一个recyclerView可以完成强大的group+children类型的业务需求。
- 每组支持添加header，footer，children，且每一个都支持设置多类型type的view视图。
- 支持局部插入刷新，局部移除刷新，也就是说可以按组插入或者移除数据，或者按组中child的某个未知插入或者移除数据。
- 支持组中header，footer，child的各个视图view的自定义点击事件。且返回具体的索引！
- 常见使用场景：仿懂车帝，汽车之家分组图片查看器；仿QQ联系人分组，可以折叠和伸展；以及复杂分组页面……
- 添加了object同步锁处理adapter中data添加，获取和移除等方法，有效避免多线程或者其他操作导致数据错位或者偶发性fast-fail。




### 02.如何使用
#### 2.1 如何引入
- 如下所示
    ```
    implementation 'com.github.yangchong211:YCGroupAdapter:1.0.6'
    ```


#### 2.2 最简单使用
- 必须的三个步骤代码，如下所示
    ```
    mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
    mAdapter = new GroupedSecondAdapter(this, list);
    mRecyclerView.setAdapter(mAdapter);
    ```
- 关于如何实现仿照QQ分组的功能
    ```
    /**
     * 判断当前组是否展开
     *
     * @param groupPosition
     * @return
     */
    public boolean isExpand(int groupPosition) {
        GroupEntity entity = mGroups.get(groupPosition);
        return entity.isExpand();
    }
    
    /**
     * 展开一个组
     *
     * @param groupPosition
     */
    public void expandGroup(int groupPosition) {
        expandGroup(groupPosition, false);
    }
    
    /**
     * 展开一个组
     *
     * @param groupPosition
     * @param animate
     */
    public void expandGroup(int groupPosition, boolean animate) {
        GroupEntity entity = mGroups.get(groupPosition);
        entity.setExpand(true);
        if (animate) {
            notifyChildrenInserted(groupPosition);
        } else {
            notifyDataChanged();
        }
    }
    
    /**
     * 收起一个组
     *
     * @param groupPosition
     */
    public void collapseGroup(int groupPosition) {
        collapseGroup(groupPosition, false);
    }
    
    /**
     * 收起一个组
     *
     * @param groupPosition
     * @param animate
     */
    public void collapseGroup(int groupPosition, boolean animate) {
        GroupEntity entity = mGroups.get(groupPosition);
        entity.setExpand(false);
        if (animate) {
            notifyChildrenRemoved(groupPosition);
        } else {
            notifyDataChanged();
        }
    }
    
    /**
     * 收起所有的组
     */
    public void collapseGroup() {
        for (int i=0 ; i<mGroups.size() ; i++){
            GroupEntity entity = mGroups.get(i);
            entity.setExpand(false);
        }
        notifyDataChanged();
    }
    ```

### 03.常用api
#### 3.1 自定义adapter
- 代码如下所示
    ```
    public class GroupedSecondAdapter extends AbsGroupedAdapter {
    
        private List<GroupEntity> mGroups;
    
        public GroupedSecondAdapter(Context context, List<GroupEntity> groups) {
            super(context);
            mGroups = groups;
        }
    
        @Override
        public int getGroupCount() {
            return mGroups == null ? 0 : mGroups.size();
        }
    
        @Override
        public int getChildrenCount(int groupPosition) {
            if (mGroups!=null){
                ArrayList<ChildEntity> children = mGroups.get(groupPosition).getChildren();
                return children == null ? 0 : children.size();
            }
            return 0;
        }
    
        @Override
        public boolean hasHeader(int groupPosition) {
            return true;
        }
    
        @Override
        public boolean hasFooter(int groupPosition) {
            return true;
        }
    
        @Override
        public int getHeaderLayout(int viewType) {
            return R.layout.item_text_header;
        }
    
        @Override
        public int getFooterLayout(int viewType) {
            return R.layout.item_text_footer;
        }
    
        @Override
        public int getChildLayout(int viewType) {
            return R.layout.item_content_view;
        }
    
        @Override
        public void onBindHeaderViewHolder(GroupViewHolder holder, int groupPosition) {
            
        }
    
        @Override
        public void onBindFooterViewHolder(GroupViewHolder holder, int groupPosition) {
            
        }
    
        @Override
        public void onBindChildViewHolder(GroupViewHolder holder, int groupPosition, int childPosition) {
            
        }
    
    }
    ```
- 那么如何控制组中的header或者footer是否显示呢？
    - 返回true表示显示，返回false表示不显示……就是这么简单
    ```
    @Override
    public boolean hasHeader(int groupPosition) {
        return true;
    }
    
    @Override
    public boolean hasFooter(int groupPosition) {
        return true;
    }
    ```

#### 3.2 notify相关
- 插入数据
    ```
    //通知一组数据插入
    mAdapter.notifyGroupInserted(1);
    //通知一个子项到组里插入
    mAdapter.notifyChildInserted(1,3);
    //通知一组里的多个子项插入
    mAdapter.notifyChildRangeInserted(1,2,10);
    //通知一组里的所有子项插入
    mAdapter.notifyChildrenInserted(1);
    //通知多组数据插入
    mAdapter.notifyGroupRangeInserted(1,3);
    //通知组头插入
    mAdapter.notifyHeaderInserted(1);
    //通知组尾插入
    mAdapter.notifyFooterInserted(1);
    ```
- 移除数据
    ```
    //通知所有数据删除
    mAdapter.notifyDataRemoved();
    //通知一组数据删除，包括组头,组尾和子项
    mAdapter.notifyGroupRemoved(1);
    //通知多组数据删除，包括组头,组尾和子项
    mAdapter.notifyGroupRangeRemoved(1,3);
    //通知组头删除
    mAdapter.notifyHeaderRemoved(1);
    //通知组尾删除
    mAdapter.notifyFooterRemoved(1);
    //通知一组里的某个子项删除
    mAdapter.notifyChildRemoved(1,3);
    //通知一组里的多个子项删除
    mAdapter.notifyChildRangeRemoved(1,3,4);
    //通知一组里的所有子项删除
    mAdapter.notifyChildrenRemoved(1);
    ```


#### 3.3 点击事件listener
- 设置组header点击事件
    ```
    mAdapter.setOnHeaderClickListener(new OnHeaderClickListener() {
        @Override
        public void onHeaderClick(AbsGroupedAdapter adapter, GroupViewHolder holder,
                                  int groupPosition) {
            Toast.makeText(SecondActivity.this,
                    "组头：groupPosition = " + groupPosition,Toast.LENGTH_LONG).show();
        }
    });
    ```
- 设置组footer点击事件
    ```
    mAdapter.setOnFooterClickListener(new OnFooterClickListener() {
        @Override
        public void onFooterClick(AbsGroupedAdapter adapter, GroupViewHolder holder,
                                  int groupPosition) {
            Toast.makeText(SecondActivity.this,
                    "组尾：groupPosition = " + groupPosition,Toast.LENGTH_LONG).show();
        }
    });
    ```
- 设置组中children点击事件
    ```
    mAdapter.setOnChildClickListener(new OnChildClickListener() {
        @Override
        public void onChildClick(AbsGroupedAdapter adapter, GroupViewHolder holder,
                                 int groupPosition, int childPosition) {
            Toast.makeText(SecondActivity.this,"子项：groupPosition = " + groupPosition
                    + ", childPosition = " + childPosition,Toast.LENGTH_LONG).show();
        }
    });
    ```


### 04.实现步骤
#### 4.1 业务需求分析
- 比如在app开发中，产品说实现一个QQ分组的功能，要求有收叠功能。同时在app中，图片相册，仿照懂车帝实现分组图片。看到这样一个需求，思考能否用一个recyclerView实现，使用type来区分不同类型布局。
- RecyclerView 可以用ViewType来区分不同的item,也可以满足需求，但还是存在一些问题，比如：
    - 1，在item过多逻辑复杂列表界面，Adapter里面的代码量庞大，逻辑复杂，后期难以维护。
    - 2，每次增加一个列表都需要增加一个Adapter，重复搬砖，效率低下。
    - 3，无法复用adapter，假如有多个页面有多个type，那么就要写多个adapter。
    - 4，要是有局部刷新，那么就比较麻烦了，比如广告区也是一个九宫格的RecyclerView，点击局部刷新当前数据，比较麻烦。


#### 4.2 adapter实现多个type
- 通常写一个多Item列表的方法
    - 根据不同的ViewType 处理不同的item，如果逻辑复杂，这个类的代码量是很庞大的。如果版本迭代添加新的需求，修改代码很麻烦，后期维护困难。
- 主要操作步骤
    - 在onCreateViewHolder中根据viewType参数，也就是getItemViewType的返回值来判断需要创建的ViewHolder类型
    - 在onBindViewHolder方法中对ViewHolder的具体类型进行判断，分别为不同类型的ViewHolder进行绑定数据与逻辑处理
- 代码如下所示
    ```
    public class HomePageAdapter extends RecyclerView.Adapter {
        public static final int TYPE_HEADER = 1;
        public static final int TYPE_FOOTER = 2;
        public static final int TYPE_IMAGE = 3;
        private List<HomePageEntry> mData;
    
        public void setData(List<HomePageEntry> data) {
            mData = data;
        }
    
        @Override
        public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            switch (viewType){
                case TYPE_HEADER:
                    return new HeaderViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.home_ad_item_layout,null));
                case TYPE_FOOTER:
                    return new FooterViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.home_text_item_layout,null));
                case TYPE_CHILD:
                    return new ChildViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.home_image_item_layout,null));
            }
            return null;
        }
    
        @Override
        public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
            int type = getItemViewType(position);
            switch (type){
                case TYPE_HEADER:
                    // TYPE_HEADER 逻辑处理
                    break;
                case TYPE_FOOTER:
                    // TYPE_FOOTER 逻辑处理
                    break;
                case TYPE_CHILD:
                    // TYPE_CHILD 逻辑处理
                    break;
            }
        }
    
        @Override
        public int getItemViewType(int position) {
             return mData.get(position).type;//type 的值为TYPE_HEADER，TYPE_FOOTER，TYPE_AD，等其中一个
        }
    
        @Override
        public int getItemCount() {
            return mData == null ? 0:mData.size();
        }
    
        public static class HeaderViewHolder extends RecyclerView.ViewHolder{
            public HeaderViewHolder(View itemView) {
                super(itemView);
                //绑定控件
            }
        }
        //省略部分代码
    }
    ```



#### 4.3 这样写的弊端
- 上面那样写的弊端
    - 类型检查与类型转型，由于在onCreateViewHolder根据不同类型创建了不同的ViewHolder，所以在onBindViewHolder需要针对不同类型的ViewHolder进行数据绑定与逻辑处理，这导致需要通过instanceof对ViewHolder进行类型检查与类型转型。
    - 不利于维护，这点应该是上一点的延伸，随着列表中布局类型的增加与变更，getItemViewType、onCreateViewHolder、onBindViewHolder中的代码都需要变更或增加，Adapter 中的代码会变得臃肿与混乱，增加了代码的维护成本。
    - 比如，在分组控件中，类似QQ分组那样，点击组中的header，可以切换关闭和伸展该组中children的自选项item，那么如果不封装，adapter对数据处理也比较麻烦。
    - 有时候，在分组控件中，有的组不想显示header，有的组不想显示footer，那么这个时候就不太灵活。能否使用一个开关方法来控制header和footer的显示和隐藏呢？


#### 4.4 分组实体bean
- 通过GroupStructure记录每个组是否有头部，是否有尾部和子项的数量。从而能方便的计算列表的长度和每个组的组头、组尾和子项在列表中的位置。


#### 4.5 构建封装adapter
- 核心目的就是三个
    - 避免类的类型检查与类型转型
    - 增强Adapter的扩展性
    - 增强Adapter的可维护性
- 当列表中类型增加或减少时Adapter中主要改动的就是getItemViewType、onCreateViewHolder、onBindViewHolder这三个方法，因此，我们就从这三个方法中开始着手。
- 在getItemViewType方法中。
    - if之类的逻辑判断简化代码，可以简单粗暴的用作为TYPE_HEADER，TYPE_FOOTER，TYPE_CHILD增加type标识。
    - 既然是分组adapter，首先是获取组的索引，然后通过组的索引来判断type的类型，最后在返回具体的itemType类型。
    ```
    @Override
    public int getItemViewType(int position) {
        itemType = position;
        int groupPosition = getGroupPositionForPosition(position);
        int type = judgeType(position);
        if (type == TYPE_HEADER) {
            return getHeaderViewType(groupPosition);
        } else if (type == TYPE_FOOTER) {
            return getFooterViewType(groupPosition);
        } else if (type == TYPE_CHILD) {
            int childPosition = getChildPositionForPosition(groupPosition, position);
            return getChildViewType(groupPosition, childPosition);
        }
        return super.getItemViewType(position);
    }

    /**
     * 判断item的type 头部 尾部 和 子项
     *
     * @param position
     * @return
     */
    public int judgeType(int position) {
        int itemCount = 0;
        //获取组的数量
        int groupCount = mStructures.size();

        for (int i = 0; i < groupCount; i++) {
            GroupStructure structure = mStructures.get(i);

            //判断是否有header头部view
            if (structure.hasHeader()) {
                itemCount += 1;
                if (position < itemCount) {
                    return TYPE_HEADER;
                }
            }

            //获取孩子的数量
            itemCount += structure.getChildrenCount();
            if (position < itemCount) {
                return TYPE_CHILD;
            }

            //判断是否有footer数量
            if (structure.hasFooter()) {
                itemCount += 1;
                if (position < itemCount) {
                    return TYPE_FOOTER;
                }
            }
        }

        //以防万一，为了避免在插入刷新，移除刷新时，避免索引越界异常，不要throw异常
        //即使当 position == getItemCount() 为true时，可以用空页面替代
        return TYPE_NO;
        //throw new IndexOutOfBoundsException("can't determine the item type of the position." +
        //        "position = " + position + ",item count = " + getItemCount());
    }
    //省略部分代码，具体可以看lib中源代码
    ```
- 在onCreateViewHolder方法中
    - 创建viewHolder，主要作用是创建Item视图，并返回相应的ViewHolder。这个地方，需要注意一下，在分组控件中，能否把组的header，footer，children等布局暴露给外部开发者创建？
    - 因此，这里需要区分类型，然后返回对应的布局，这里返回对应的布局几个方法，可以弄成抽象的方法，子类必须实现。让子类返回具体的header，footer，children布局。
    ```
    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view;
        if (viewType != TYPE_NO){
            int layoutId = getLayoutId(itemType, viewType);
            if (inflater==null){
                inflater = LayoutInflater.from(mContext);
            }
            view = inflater.inflate(layoutId, parent, false);
        } else {
            //使用空布局
            //未知类型可以使用空布局代替
            view = new View(parent.getContext());
        }
        return new GroupViewHolder(view);
    }
    
    private int getLayoutId(int position, int viewType) {
        int type = judgeType(position);
        if (type == TYPE_HEADER) {
            return getHeaderLayout(viewType);
        } else if (type == TYPE_FOOTER) {
            return getFooterLayout(viewType);
        } else if (type == TYPE_CHILD) {
            return getChildLayout(viewType);
        }
        return 0;
    }
    ```
- 在onBindViewHolder方法中
    - 这个方法中主要做两个事情，第一个是设置组中的header，footer，还有children的点击事件，并且需要返回具体的索引，包括组索引，和组中孩子的索引。
    - 第二个是绑定viewHolder，主要作用是绑定数据到正确的Item视图上，这个可以把方法抽象，让子类去实现。
    ```
    @Override
    public void onBindViewHolder(@NonNull final RecyclerView.ViewHolder holder, int position) {
        int type = judgeType(position);
        final int groupPosition = getGroupPositionForPosition(position);
        if (type == TYPE_HEADER) {
            if (mOnHeaderClickListener != null) {
                holder.itemView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mOnHeaderClickListener != null) {
                            mOnHeaderClickListener.onHeaderClick(AbsGroupAdapter.this,
                                    (GroupViewHolder) holder, groupPosition);
                        }
                    }
                });
            }
            onBindHeaderViewHolder((GroupViewHolder) holder, groupPosition);
        } else if (type == TYPE_FOOTER) {
            if (mOnFooterClickListener != null) {
                holder.itemView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mOnFooterClickListener != null) {
                            mOnFooterClickListener.onFooterClick(AbsGroupAdapter.this,
                                    (GroupViewHolder) holder, groupPosition);
                        }
                    }
                });
            }
            onBindFooterViewHolder((GroupViewHolder) holder, groupPosition);
        } else if (type == TYPE_CHILD) {
            final int childPosition = getChildPositionForPosition(groupPosition, position);
            if (mOnChildClickListener != null) {
                holder.itemView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mOnChildClickListener != null) {
                            mOnChildClickListener.onChildClick(AbsGroupAdapter.this,
                                    (GroupViewHolder) holder, groupPosition, childPosition);
                        }
                    }
                });
            }
            onBindChildViewHolder((GroupViewHolder) holder, groupPosition, childPosition);
        }
    }
    ```
- 封装后好处
    - 拓展性——每组支持添加header，footer，children，且每一个都支持设置多类型type的view视图。而且支持局部插入刷新，局部移除刷新，也就是说可以按组插入或者移除数据，或者按组中child的某个未知插入或者移除数据。
    - 可维护性——不同的列表类型由adapter添加header，footer，children类型处理，相互之间互不干扰，代码简洁，维护成本低。还可以灵活控制header，footer类型的布局是否可见，特别灵活！




#### 其他推荐说明
- 1.[技术博客汇总](https://www.jianshu.com/p/614cb839182c)
- 2.[开源项目汇总](https://blog.csdn.net/m0_37700275/article/details/80863574)
- 3.[生活博客汇总](https://blog.csdn.net/m0_37700275/article/details/79832978)
- 4.[喜马拉雅音频汇总](https://www.jianshu.com/p/f665de16d1eb)
- 5.[其他汇总](https://www.jianshu.com/p/53017c3fc75d)
- 6.[重点推荐：博客笔记大汇总，开源文件都是md格式](https://github.com/yangchong211/YCBlogs)



#### 关于LICENSE
```
Copyright 2017 yangchong211（github.com/yangchong211）

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

