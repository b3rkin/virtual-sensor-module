using UnityEngine;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.IO;

public class RecordedDataSender : MonoBehaviour
{
    public string ipAddress = "127.0.0.1"; // IP address of the destination
    public int port = 1234; // Port number of the destination
    private UdpClient udpClient;
    private byte[] packetBuffer;

    public string csvFilePath = "Assets/Resources/ReplayData.csv";  // Path to the CSV file

    private StreamReader reader;
    private bool isReplaying = false;


    private void Start()
    {
        udpClient = new UdpClient();
        packetBuffer = new byte[52];

        
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Y) && !isReplaying)
        {
            StartReplaying();
        }
        else if (Input.GetKeyDown(KeyCode.Y) && isReplaying){
            Debug.Log("Replaying stopped");
            reader.Close();
            reader = null;
            isReplaying = false;
        }
    }

    private void FixedUpdate()
    {
        if (isReplaying && !reader.EndOfStream)
        {
            try{
                string line = reader.ReadLine();
                string[] values = line.Split(',');

                float posX  = float.Parse(values[0]);
                float posY  = float.Parse(values[1]);
                float posZ  = float.Parse(values[2]);
                float rotX = float.Parse(values[3]);
                float rotY = float.Parse(values[4]);
                float rotZ = float.Parse(values[5]);
                float angVx = float.Parse(values[6]);
                float angVy = float.Parse(values[7]);
                float angVz = float.Parse(values[8]);
                float velX = float.Parse(values[9]);
                float velY = float.Parse(values[10]);
                float velZ = float.Parse(values[11]);
                uint timestamp = uint.Parse(values[12]);

                byte[] xPositionBytes = BitConverter.GetBytes(posX);
                byte[] yPositionBytes = BitConverter.GetBytes(posY);
                byte[] zPositionBytes = BitConverter.GetBytes(posZ);
                byte[] xRotationBytes = BitConverter.GetBytes(rotX );
                byte[] yRotationBytes = BitConverter.GetBytes(rotY );
                byte[] zRotationBytes = BitConverter.GetBytes(rotZ );  
                byte[] angularVelocityBytesX = BitConverter.GetBytes(angVx);
                byte[] angularVelocityBytesY = BitConverter.GetBytes(angVy);
                byte[] angularVelocityBytesZ = BitConverter.GetBytes(angVz);
                byte[] velocityBytesX = BitConverter.GetBytes(velX);
                byte[] velocityBytesY = BitConverter.GetBytes(velY);
                byte[] velocityBytesZ = BitConverter.GetBytes(velZ);
                byte[] timestampBytes = BitConverter.GetBytes(timestamp);

                // Copy position and orientation bytes to the packet buffer
                Array.Copy(xPositionBytes, 0, packetBuffer, 0, 4);
                Array.Copy(yPositionBytes, 0, packetBuffer, 4, 4);
                Array.Copy(zPositionBytes, 0, packetBuffer, 8, 4);
                Array.Copy(xRotationBytes, 0, packetBuffer, 12, 4);
                Array.Copy(yRotationBytes, 0, packetBuffer, 16, 4);
                Array.Copy(zRotationBytes, 0, packetBuffer, 20, 4);
                Array.Copy(angularVelocityBytesX, 0, packetBuffer, 24, 4);
                Array.Copy(angularVelocityBytesY, 0, packetBuffer, 28, 4);
                Array.Copy(angularVelocityBytesZ, 0, packetBuffer, 32, 4);
                Array.Copy(velocityBytesX, 0, packetBuffer, 36, 4);
                Array.Copy(velocityBytesY, 0, packetBuffer, 40, 4);
                Array.Copy(velocityBytesZ, 0, packetBuffer, 44, 4);
                Array.Copy(timestampBytes, 0, packetBuffer, 48, sizeof(uint));

                // Send the UDP packet to the destination
                udpClient.Send(packetBuffer, packetBuffer.Length, ipAddress, port);
            }
            catch (Exception e)
            {
                Debug.Log(e.Message+"\n");
                Debug.Log("Replaying stopped");
                reader.Close();
                reader = null;
                
                isReplaying = false;
            }
            
        }
    }

    private void StartReplaying()
    {
        Debug.Log("Replaying started!");
        if (!string.IsNullOrEmpty(csvFilePath))
        {
            reader = new StreamReader(csvFilePath);
        }
        isReplaying = true;
    }

    private void OnApplicationQuit()
    {
        // Close the UDP client when the application quits
        udpClient.Close();
    }
}
