package io.github.controlwear.joystickdemo;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;

import io.github.controlwear.virtual.joystick.android.JoystickView;

public class MainActivity extends AppCompatActivity {


    private TextView mTextViewAngleLeft;
    private TextView mTextViewStrengthLeft;

    private TextView mTextViewAngleRight;
    private TextView mTextViewStrengthRight;
    private TextView mTextViewCoordinateRight;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        final Button button = findViewById(R.id.butonFaruri);
        button.setOnClickListener(new View.OnClickListener() {
            @SuppressLint("DefaultLocale")
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                try {
                    String toSend = "far";
                    DatagramSocket clientSocket = new DatagramSocket();
                    InetAddress IPAddress = InetAddress.getByName("192.168.0.173");
                    byte[] sendData = new byte[1024];
                    ByteBuffer b = ByteBuffer.allocate(4);
                    sendData = toSend.getBytes();
                    DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 4210);
                    sendPacket.setLength(sendData.length);
                    clientSocket.send(sendPacket);
                } catch (Exception e) {
                }
                    }
                }).start();
            }
        });


//        public void switchFar(View view) {
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    try {
//                        String toSend = "far:switch";
//                        DatagramSocket clientSocket = new DatagramSocket();
//                        InetAddress IPAddress = InetAddress.getByName("192.168.0.173");
//                        byte[] sendData = new byte[1024];
//                        ByteBuffer b = ByteBuffer.allocate(4);
//                        sendData = toSend.getBytes();
//                        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 4210);
//                        sendPacket.setLength(sendData.length);
//                        clientSocket.send(sendPacket);
//                    } catch (Exception e) {
//                    }
//                }
//            }).start();
//        }

        mTextViewAngleRight = (TextView) findViewById(R.id.textView_angle_right);
        mTextViewStrengthRight = (TextView) findViewById(R.id.textView_strength_right);
        mTextViewCoordinateRight = findViewById(R.id.textView_coordinate_right);

        final JoystickView joystickRight = (JoystickView) findViewById(R.id.joystickView_right);
        joystickRight.setOnMoveListener(new JoystickView.OnMoveListener() {
            @SuppressLint("DefaultLocale")
            @Override
            public void onMove(final int angle, final int strength) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            String toSend = angle + "-" + strength; //+ "-" + joystickRight.getNormalizedX() + "/" + joystickRight.getNormalizedY();
                            DatagramSocket clientSocket = new DatagramSocket();
                            InetAddress IPAddress = InetAddress.getByName("192.168.0.173");
                            byte[] sendData = new byte[1024];
                            ByteBuffer b = ByteBuffer.allocate(4);
                            sendData = toSend.getBytes();
                            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 4210);
                            sendPacket.setLength(sendData.length);
                            clientSocket.send(sendPacket);
                            mTextViewAngleRight.setText(angle + "°");
                            mTextViewStrengthRight.setText(strength + "%");
                            mTextViewCoordinateRight.setText(
                                String.format("x%03d:y%03d",
                                    joystickRight.getNormalizedX(),
                                    joystickRight.getNormalizedY())
                            );
                        } catch (Exception e) {
                        }
                    }
                }).start();
            }
        });



//        public void sendInfo(View view) {
//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    try {
////                    HelloText.setText("tfi");
//                        TextView info = findViewById(R.id.joystickView_right);
//                        String toSend = message.getText().toString();
//                        DatagramSocket clientSocket = new DatagramSocket();
//                        InetAddress IPAddress = InetAddress.getByName("192.168.0.173");
//                        byte[] sendData = new byte[1024];
//                        sendData = toSend.getBytes();
//                        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 4210);
//                        sendPacket.setLength(sendData.length);
//                        clientSocket.send(sendPacket);
//                    }
//                    catch (Exception e) { }
//                }
//            }).start();
//        }

    }
}
