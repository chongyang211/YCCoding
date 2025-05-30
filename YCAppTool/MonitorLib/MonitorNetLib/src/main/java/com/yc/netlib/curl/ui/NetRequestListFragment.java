package com.yc.netlib.curl.ui;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.yc.eastadapterlib.OnItemClickListener;
import com.yc.netlib.R;
import com.yc.netlib.curl.data.IDataPoolHandleImpl;
import com.yc.netlib.curl.data.NetworkFeedBean;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;

public class NetRequestListFragment extends Fragment {

    private RecyclerView mRecyclerView;
    private Activity activity;

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        activity = (Activity) context;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {
        View inflate = inflater.inflate(R.layout.base_recycler_view, container, false);
        return inflate;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initRecyclerView(view);
    }

    private void initRecyclerView(View view) {
        mRecyclerView = view.findViewById(R.id.recyclerView);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(activity);
        mRecyclerView.setLayoutManager(linearLayoutManager);
        NetworkFeedAdapter adapter = new NetworkFeedAdapter(activity);
        mRecyclerView.setAdapter(adapter);
        final List<NetworkFeedBean> mNetworkFeedList = new ArrayList<>();
        HashMap<String, NetworkFeedBean> networkFeedMap = IDataPoolHandleImpl.getInstance().getNetworkFeedMap();
        if (networkFeedMap != null) {
            Collection<NetworkFeedBean> values = networkFeedMap.values();
            mNetworkFeedList.addAll(values);
            try {
                Collections.sort(mNetworkFeedList, new Comparator<NetworkFeedBean>() {
                    @Override
                    public int compare(NetworkFeedBean networkFeedModel1, NetworkFeedBean networkFeedModel2) {
                        return (int) (networkFeedModel2.getCreateTime() - networkFeedModel1.getCreateTime());
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        adapter.setData(mNetworkFeedList);
        adapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {
                NetworkFeedBean networkFeedBean = mNetworkFeedList.get(position);
                NetworkDetailActivity.start(activity, networkFeedBean.getRequestId());
            }
        });
    }
}
