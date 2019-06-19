package com.example.skichenaradjou.station_final;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothHeadset;
import android.content.Intent;
import android.graphics.Typeface;
import android.graphics.drawable.AnimationDrawable;
import android.support.constraint.ConstraintLayout;
import android.support.v4.content.res.ResourcesCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class Bluetooth extends AppCompatActivity {

    /**** Déclaration des variables et des objets  ****/
    ConstraintLayout myLayout;
    AnimationDrawable animationDrawable;

    Button btnPaired;
    ListView devicelist;

    private BluetoothAdapter myBluetooth = null;
    private Set<BluetoothDevice> pairedDevices;
    public static String EXTRA_ADDRESS = "device_address";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        /****    Animation   ****/

        myLayout = findViewById(R.id.myLayout);
        animationDrawable = (AnimationDrawable) myLayout.getBackground();
        animationDrawable.setEnterFadeDuration(4500);
        animationDrawable.setExitFadeDuration(4500);
        animationDrawable.start();

        /**** Fin de l'animation du background  ****/


        /**** Asscociation des widget dans les objets  ****/

        btnPaired = (Button) findViewById(R.id.bt);
        devicelist = (ListView) findViewById(R.id.listView);

        devicelist.setVisibility(View.INVISIBLE);

        /****  On cherche à récupérer l'interface bluetooth du périphérique Android ****/
        myBluetooth = BluetoothAdapter.getDefaultAdapter();


        /**** Vérifie si le device est en possession d'un dispositif bluetooth  ****
         **** Si pas de module (d'interface) bluetooth sur le périphérique ...    ****/
        if ( myBluetooth==null ) {
            Toast.makeText(getApplicationContext(), "Bluetooth device not available", Toast.LENGTH_LONG).show();
            finish();
        } else if ( !myBluetooth.isEnabled() ) {
            // On récupère le périphérique bluetooth détecté durant le scan
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnBTon, 1);
        }

        btnPaired.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /**** Dès qu'on appuie sur le bouton, il nous ouvre une liste de device
                      avec qui on a déjà apparé en Bluetooth ********/
                pairedDevicesList();
                devicelist.setVisibility(View.VISIBLE);
            }
        });
    }

    /*****              FONCTION QUI PERMET DE RETOURNER UNE LISTE DE DEVICE        *******/

    private void pairedDevicesList () {

        /**** récupérer la liste des appareils déjà appairé  ****/
        pairedDevices = myBluetooth.getBondedDevices();
        ArrayList list = new ArrayList();

        if ( pairedDevices.size() > 0 ) {
            for ( BluetoothDevice bt : pairedDevices ) {
                list.add(bt.getName().toString() + "\n" + bt.getAddress().toString());
            }
        } else {
            Toast.makeText(getApplicationContext(), "No Paired Bluetooth Devices Found.", Toast.LENGTH_LONG).show();
        }

        final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, list);
        devicelist.setAdapter(adapter);
        devicelist.setOnItemClickListener(myListClickListener);
    }

    /****                             FIN DE LA FONCTION                               ****/


    /**** DES QU"ON APPUIE SUR UN DE CES DEVICE SUR LA LISTE ON PASSE A L'AUTRE ACTIVITE ET ENVOIE L'ADRESSE ****/

    private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            String info = ((TextView) view).getText().toString();
            String address = info.substring(info.length()-17);

            Intent i = new Intent(Bluetooth.this,Station.class);
            i.putExtra(EXTRA_ADDRESS, address);
            startActivity(i);
        }
    };
}