package com.example.skichenaradjou.station_final;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

public class Menu extends AppCompatActivity {

    /**** Déclaration des variables et des objets  ****/

    private TextView tv;
    private ImageView iv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /**** permet de gérer la fenêtre de l'application  ****/
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_menu);

        /**** Asscociation des widget dans les objets                    ****
         **** On enregistre un gestionnaire d'événements sur les widgets ****/
        tv = findViewById(R.id.titre);
        iv = findViewById(R.id.imageView);

        /**** Permet de charger l'animation sur les widget,ici une animation en alpha ****/

        Animation myanim = AnimationUtils.loadAnimation(this, R.anim.transition);
        tv.startAnimation(myanim);
        iv.startAnimation(myanim);

        /****  Un thread qui permet de laisser le temps à l'animation de se finir
               et de passer à l'autre activité ****/

        final Intent i = new Intent(Menu.this,  Bluetooth.class);
        Thread timer = new Thread() {
            public void run () {
                try {
                    sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                finally {
                    startActivity(i);
                    finish();
                }
            }
        };
        timer.start();
    }
}

