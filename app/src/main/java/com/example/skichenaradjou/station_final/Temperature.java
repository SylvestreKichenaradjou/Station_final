package com.example.skichenaradjou.station_final;

import android.graphics.Color;
import android.graphics.drawable.AnimationDrawable;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.support.constraint.ConstraintLayout;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.Utils;

import java.text.DecimalFormat;
import java.util.ArrayList;

import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class Temperature extends AppCompatActivity {


    /**** Déclaration des variables et des objets  ****/
    LineChart mChart;
    TextView moyenne;

    float resultat = 0;

    ArrayList<Entry> yValues = new ArrayList<>();
    ArrayList<String> array = new ArrayList<String>();
    ArrayList<Float> conversion = new ArrayList<Float>();

    ConstraintLayout myLayout;
    AnimationDrawable animationDrawable;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_temperature);

        myLayout = findViewById(R.id.graph_temperature);
        moyenne = findViewById(R.id.textView);

        animationDrawable = (AnimationDrawable) myLayout.getBackground();
        animationDrawable.setEnterFadeDuration(4500);
        animationDrawable.setExitFadeDuration(4500);
        animationDrawable.start();

        mChart = findViewById(R.id.linechart);

        /****    Configurer le Linechart   ****/
        mChart.setDragEnabled(true);
        mChart.setScaleEnabled(true);
        mChart.setDrawGridBackground(false);
        mChart.setPinchZoom(true);

        /****    Limiter les ordonnées    ****/
        YAxis yAxisRight = mChart.getAxisRight();
        YAxis yAxisLeft = mChart.getAxisLeft();

        yAxisRight.setAxisMinimum(0);
        yAxisRight.setAxisMaximum(50);
        yAxisLeft.setAxisMinimum(0);
        yAxisLeft.setAxisMaximum(50);


        /****    Récupérer les donnnées de l'activité Station.java    ****/
        Intent intent = getIntent();

        if (intent != null) {

            if (intent.hasExtra("recup_temperature")) {
                Bundle b = getIntent().getExtras();
                array = (ArrayList<String>) b.getStringArrayList("recup_temperature");
            }

        }

        /****    Conversion des string en float    ****/
        for (int i = 0; i < array.size(); i++) {
            conversion.add(Float.valueOf(array.get(i)));
        }


        for (int i = 0; i < conversion.size(); i++) {
            resultat = (conversion.get(i) + resultat);
            yValues.add(new Entry(i, conversion.get(i)));
        }

        DecimalFormat f = new DecimalFormat();
        f.setMaximumFractionDigits(2);
        moyenne.setText("Moyenne : " + f.format((double) resultat / conversion.size()) + "°C");


        /****    Configurer les datas    ****/
        LineDataSet set1 = new LineDataSet(yValues, "");

        set1.setFillAlpha(110);

        set1.setDrawFilled(true);
        if (Utils.getSDKInt() >= 18) {
            // fill drawable only supported on api level 18 and above
            Drawable drawable = ContextCompat.getDrawable(this, R.drawable.red_blue);
            set1.setFillDrawable(drawable);
        } else {
            set1.setFillColor(Color.BLACK);
        }


        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
        dataSets.add(set1);

        LineData data = new LineData(dataSets);
        mChart.setScaleMinima(0, 0);
        mChart.animateY(2000);
        mChart.setData(data);
    }
}
