package com.example.skichenaradjou.station_final;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.AnimationDrawable;
import android.graphics.drawable.Drawable;
import android.support.constraint.ConstraintLayout;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.Utils;

import java.text.DecimalFormat;
import java.util.ArrayList;

public class Humidite extends AppCompatActivity {


    /**** Déclaration des variables et des objets  ****/
    TextView moyenne;
    float resultat = 0;

    ArrayList<BarEntry> yValues = new ArrayList<>();
    ArrayList<String> array = new ArrayList<String>();
    ArrayList<Float> conversion = new ArrayList<Float>();

    ConstraintLayout myLayout;
    AnimationDrawable animationDrawable;

    private BarChart mChart;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_humidite);

        myLayout = findViewById(R.id.graph_humidite);
        moyenne = findViewById(R.id.moyenne_humide);
        mChart = findViewById(R.id.chart);

        animationDrawable = (AnimationDrawable) myLayout.getBackground();
        animationDrawable.setEnterFadeDuration(4500);
        animationDrawable.setExitFadeDuration(4500);
        animationDrawable.start();

        /****    Configurer le Barchart   ****/
        mChart.setDragEnabled(true);
        mChart.setScaleEnabled(true);
        mChart.setDrawGridBackground(false);
        mChart.setPinchZoom(true);

        /****    Limiter les ordonnées    ****/
        YAxis yAxisRight = mChart.getAxisRight();
        YAxis yAxisLeft = mChart.getAxisLeft();

        yAxisRight.setAxisMinimum(0);
        yAxisRight.setAxisMaximum(100);
        yAxisLeft.setAxisMinimum(0);
        yAxisLeft.setAxisMaximum(100);

        /****    Récupérer les donnnées de l'activité Station.java    ****/
        Intent intent = getIntent();

        if (intent != null) {

            if (intent.hasExtra("recup_humidite")) {
                Bundle b = getIntent().getExtras();
                array = (ArrayList<String>) b.getStringArrayList("recup_humidite");
            }

        }
        /****    Conversion des string en float    ****/
        for (int i = 0; i < array.size(); i++) {
            conversion.add(Float.valueOf(array.get(i)));
        }


        for (int i = 0; i < conversion.size(); i++) {
            resultat = (conversion.get(i) + resultat);
            yValues.add(new BarEntry(i, conversion.get(i)));
        }

        DecimalFormat f = new DecimalFormat();
        f.setMaximumFractionDigits(2);
        moyenne.setText("Moyenne : " + f.format((double) resultat / conversion.size()) + "%");

        /****    Configurer les datas    ****/
        BarDataSet set1 = new BarDataSet(yValues, "");
        set1.setColor(Color.CYAN);

        set1.setDrawValues(true);
        BarData data = new BarData(set1);

        mChart.setScaleMinima(0, 0);
        mChart.animateY(2000);
        mChart.setData(data);
    }
}
