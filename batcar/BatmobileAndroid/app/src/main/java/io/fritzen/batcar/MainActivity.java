package io.fritzen.batcar;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

import io.github.controlwear.virtual.joystick.android.JoystickView;

public class MainActivity extends AppCompatActivity {


    private TextView mTextViewAngleLeft;
    private TextView mTextViewStrengthLeft;

    private TextView mTextViewAngleRight;
    private TextView mTextViewStrengthRight;
    private TextView mTextViewCoordinateRight;

    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    String address = null;

    public static final int STICK_NONE = 0;
    public static final int STICK_UP = 1;
    public static final int STICK_UPRIGHT = 2;
    public static final int STICK_RIGHT = 3;
    public static final int STICK_DOWNRIGHT = 4;
    public static final int STICK_DOWN = 5;
    public static final int STICK_DOWNLEFT = 6;
    public static final int STICK_LEFT = 7;
    public static final int STICK_UPLEFT = 8;


    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private void Disconnect()
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.close(); //close connection
            }
            catch (IOException e)
            { msg("Error");}
        }
        finish(); //return to the first layout

    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }
    private void sendBtMsg(String msg)
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write(msg.getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void send(int direction, int strength) {

        Switch boost =  (Switch) findViewById(R.id.switch1);

        int outMax = 6; //70%
        if (boost.isChecked())
        {
            outMax = 9; //100%
        }
        strength = map(strength, 0,100, 0,outMax);
        String cmd = "D" + String.valueOf(direction) + "S" + String.valueOf(strength);
        Log.d("BTMSG", cmd);
        sendBtMsg(cmd);

    }

    int  map(int x, int in_min, int in_max, int out_min, int out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS); //receive the address of the bluetooth device
        new ConnectBTX().execute(); //Call the class to connect




        mTextViewAngleRight = (TextView) findViewById(R.id.textView_angle_right);
        mTextViewStrengthRight = (TextView) findViewById(R.id.textView_strength_right);
        mTextViewCoordinateRight = (TextView) findViewById(R.id.textView_coordinate_right);

        final JoystickView joystickRight = (JoystickView) findViewById(R.id.joystickView_right);

        joystickRight.setOnMoveListener(new JoystickView.OnMoveListener() {
            @SuppressLint("DefaultLocale")
            @Override
            public void onMove(int angle, int strength) {


                mTextViewAngleRight.setText(angle + "°");
                mTextViewStrengthRight.setText(strength + "%");
                mTextViewCoordinateRight.setText(
                        String.format("x%03d:y%03d",
                                joystickRight.getNormalizedX(),
                                joystickRight.getNormalizedY())
                );
                if (strength < 15) {
                    send(STICK_NONE, strength);
                    mTextViewCoordinateRight.setText("STICK_NONE - " + strength);

                }
                if (strength >= 15) {
                    //ANGLULO ~22 GRAUS
                    if (angle >= 331 || angle <= 22) {
                        send(STICK_RIGHT, strength);
                        mTextViewCoordinateRight.setText("STICK_RIGHT - " + strength);
                    } else if (angle >= 22 && angle <= 67) {
                        send(STICK_UPRIGHT, strength);
                        mTextViewCoordinateRight.setText("STICK_UPRIGHT - " + strength);
                    } else if (angle >= 67 && angle <= 111) {
                        send(STICK_UP, strength);
                        mTextViewCoordinateRight.setText("STICK_UP - " + strength);
                    } else if (angle >= 111 && angle <= 155) {
                        send(STICK_UPLEFT, strength);
                        mTextViewCoordinateRight.setText("STICK_UPLEFT - " + strength);
                    } else if (angle >= 155 && angle <= 199) {
                        send(STICK_LEFT, strength);
                        mTextViewCoordinateRight.setText("STICK_LEFT - " + strength);
                    } else if (angle >= 199 && angle <= 243) {
                        send(STICK_DOWNLEFT, strength);
                        mTextViewCoordinateRight.setText("STICK_DOWNLEFT - " + strength);
                    } else if (angle >= 243 && angle <= 287) {
                        send(STICK_DOWN, strength);
                        mTextViewCoordinateRight.setText("STICK_DOWN - " + strength);
                    } else if (angle >= 287 && angle <= 331) {
                        send(STICK_DOWNRIGHT, strength);
                        mTextViewCoordinateRight.setText("STICK_DOWNRIGHT - " + strength);
                    } else {
                        mTextViewCoordinateRight.setText("ERROR - " + strength);
                    }
                }
            }
        });
    }

    private class ConnectBTX extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(MainActivity.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            }
            else
            {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }
}