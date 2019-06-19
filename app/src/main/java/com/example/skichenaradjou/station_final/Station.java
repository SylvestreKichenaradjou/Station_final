package com.example.skichenaradjou.station_final;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.drawable.AnimationDrawable;
import android.os.AsyncTask;
import android.os.Handler;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;

public class Station extends AppCompatActivity {

    public final static String EXTRA_MESSAGE = "com.ltm.ltmactionbar.MESSAGE";
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    /**** Des String qui permet de récuperer les données lues ****/

    final String nombre_tempCarte[]= new String[1000];
    final String nombre_tempExt[]= new String[1000];
    final String nombre_humide[] = new String[1000];
    final String nombre_girouette[]=new String[1000];
    final String nombre_horloge[]=new String[1000];
    final String nombre_vitesse[]=new String[1000];

    /**** Déclaration des variables et des objets  ****/

    TextView celsius;
    TextView celsius_vir;
    TextView humide;
    TextView humide_vir;
    TextView direction;
    TextView horloge;
    TextView vent;
    TextView vent_vir;
    TextView temp_carte;
    ImageView menu_temperature;
    ImageView menu_humidite;
    ImageView imageView;

    ConstraintLayout myLayout;
    AnimationDrawable animationDrawable;

    String address = null;

    OutputStream mmOutputStream;
    InputStream mmInputStream;

    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    volatile boolean stopWorker;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    int tc=0,te=0,h=0,g=0,hor=0,v=0;

    ArrayList<String> recup_temperature = new ArrayList<String>();
    ArrayList<String> recup_humidite = new ArrayList<String>();
    private ProgressDialog progress;
    private boolean isBtConnected = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_station);

        /****    Animation   ****/
        myLayout = findViewById(R.id.station_menu);

        animationDrawable = (AnimationDrawable) myLayout.getBackground();
        animationDrawable.setEnterFadeDuration(4500);
        animationDrawable.setExitFadeDuration(4500);
        animationDrawable.start();

        /**** Asscociation des widget dans les objets  ****/
        celsius = findViewById(R.id.celsius);
        celsius_vir = findViewById(R.id.celsius_virgule);
        humide = findViewById(R.id.humide);
        humide_vir = findViewById(R.id.humide_virgule);
        vent = findViewById(R.id.vent);
        vent_vir = findViewById(R.id.vent_virgule);
        direction = findViewById(R.id.direction);
        horloge = findViewById(R.id.horloge);
        temp_carte = findViewById(R.id.texte_carte);

        menu_temperature = findViewById(R.id.img_temperature);
        menu_humidite = findViewById(R.id.img_humidite);


        /*** reçoit l'adresse du périphérique Bluetooth ****/
        Intent newint = getIntent();
        address = newint.getStringExtra(Bluetooth.EXTRA_ADDRESS);

        final Intent monIntent_temperature = new Intent().setClass(this, Temperature.class);
        final Intent monIntent_humidite = new Intent().setClass(this, Humidite.class);

        /**** Lancer la connexion via la méthode ****/
        new ConnectBT().execute();

        /**** Envoyer les données de température et d'humidité dans des sous activités ****/

        monIntent_temperature.putExtra("recup_temperature", recup_temperature);
        monIntent_humidite.putExtra("recup_humidite", recup_humidite);


        /**** Cliquer sur la zone permet de passer dans l'activité ****/

        menu_temperature.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(monIntent_temperature);
            }
        });

        menu_humidite.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(monIntent_humidite);
            }
        });

    }

    private void msg(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    /**** FONCTION QUI PERMET DE RECUPERER LES DONNEES EN BLUETOOTH ****/

    public void beginListenForData()
    {
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable()
        {
            public void run()
            {
                while(!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = mmInputStream.available();
                        if(bytesAvailable > 0)
                        {
                            byte[] packetBytes = new byte[bytesAvailable];
                            /** lire les octets du tampon **/
                            mmInputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable()
                                    {
                                        public void run()
                                        {
                                            /** On coupe ce qu'on reçoit pour séparer les données **/
                                            if(data.contains("/TE"))
                                            {
                                                nombre_tempExt[te]= data.substring(data.indexOf("E") + 1, data.indexOf("./"));
                                                recup_temperature.add(nombre_tempExt[te]);
                                                String nombre_text_temp[] = nombre_tempExt[te].split("\\.");
                                                celsius.setText(nombre_text_temp[0]);
                                                celsius_vir.setText("." + nombre_text_temp[1]);
                                                if(tc==1000){tc=0;}else{tc++;}
                                            }
                                            if(data.contains("/H"))
                                            {
                                                nombre_humide[h]= data.substring(data.indexOf("H") + 1, data.indexOf("./"));
                                                recup_humidite.add(nombre_humide[h]);
                                                String nombre_text_humide[] = nombre_humide[h].split("\\.");
                                                humide.setText(nombre_text_humide[0]);
                                                humide_vir.setText("." + nombre_text_humide[1]);
                                                if(h==1000){h=0;}else{h++;}
                                            }
                                            if(data.contains("/G"))
                                            {
                                                nombre_girouette[g]=data.substring(data.indexOf("G")+1,data.indexOf("./"));
                                                direction.setText(nombre_girouette[g]);
                                                if(g==1000){g=0;}else{g++;}
                                            }
                                            if(data.contains("/TC"))
                                            {
                                                nombre_tempCarte[tc]=data.substring(data.indexOf("C")+1,data.indexOf("./"));
                                                temp_carte.setText("Température de la carte : " + nombre_tempCarte[tc] + "°C");
                                                if(tc==1000){tc=0;}else{tc++;}
                                            }
                                            if(data.contains("/D"))
                                            {
                                                nombre_horloge[hor]=data.substring(data.indexOf("D")+1,data.indexOf("./"));
                                                horloge.setText(nombre_horloge[hor]);
                                                if(hor==1000){hor=0;}else{hor++;}
                                            }
                                            if(data.contains("/V"))
                                            {
                                                nombre_vitesse[v]=data.substring(data.indexOf("V")+1,data.indexOf("./"));
                                                String nombre_texte_vent[] = nombre_vitesse[v].split("\\.");
                                                vent.setText(nombre_texte_vent[0]);
                                                vent_vir.setText("." + nombre_texte_vent[1]);
                                                if(te==1000){te=0;}else{te++;}
                                            }

                                            Log.i("Main2Activity", "Valeur : " + recup_temperature);
                                            Log.i("Main2Activity", "Valeur : " + recup_humidite);

                                        }
                                    });

                                }

                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex)
                    {
                        stopWorker = true;
                    }
                }
            }
        });

        workerThread.start();

    }

    /**** FIN DE FONCTION ****/

    /****  Les AsyncTask permettent une utilisation correcte et facile du ThreadUI. *******/

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSuccess = true;

        /**** thread d’interface avant que la tâche soit exécutée *****/
        /****  montre une fenetre de dialogue ****/
        @Override
        protected void onPreExecute() {
            progress = ProgressDialog.show(Station.this, "Connexion en cours", "Patientez s'il vous plait...");
        }


        /****   invoqué sur le thread en arrière-plan immédiatement après OnPreExecute () ****/
        /**** Pendant que le progress dialog s'affiche, la connexion se fait en arrière plan ***/
        @Override
        protected Void doInBackground(Void... devices) {
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositive = myBluetooth.getRemoteDevice(address);
                    btSocket = dispositive.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    /*** Connexion ***/
                    btSocket.connect();
                    mmOutputStream = btSocket.getOutputStream();
                    mmInputStream = btSocket.getInputStream();
                }
            } catch (IOException e) {
                ConnectSuccess = false;
            }

            return null;
        }

        /****  appelé sur le thread UI après la fin du calcul  ****/

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);

            if (!ConnectSuccess) {
                msg("La connexion a échoué");
                finish();
            } else {
                msg("Connecté !");
                isBtConnected = true;;
                beginListenForData();
            }

            progress.dismiss();
        }
    }


}