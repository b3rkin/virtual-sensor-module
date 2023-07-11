using UnityEngine;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

public class UDPSender : MonoBehaviour
{
    public string ipAddress = "127.0.0.1"; // IP address of the destination
    public int port = 1234; // Port number of the destination

    private UdpClient udpClient;
    private byte[] packetBuffer;
    private Rigidbody rb;

    private void Start()
    {
        udpClient = new UdpClient();
        // packetBuffer = new byte[28];
        packetBuffer = new byte[44];
        rb = GetComponent<Rigidbody>();
        
    }

    private void FixedUpdate()
    {
        // Get the position and orientation of the object
        Vector3 position = transform.localPosition;
        Quaternion rotation = transform.rotation;
        // Vector3 angularVelocity = rb.angularVelocity;
        Vector3 angularVelocity = transform.InverseTransformDirection(rb.angularVelocity);
        // Convert position and orientation to bytes
        byte[] xPositionBytes = BitConverter.GetBytes(position.x);
        byte[] yPositionBytes = BitConverter.GetBytes(position.z);
        byte[] zPositionBytes = BitConverter.GetBytes(position.y);
        // byte[] xRotationBytes = BitConverter.GetBytes(Mathf.Abs(360 - rotation.eulerAngles.x)%360 );
        // byte[] yRotationBytes = BitConverter.GetBytes(Mathf.Abs(360 - rotation.eulerAngles.z)%360 );
        // byte[] zRotationBytes = BitConverter.GetBytes(Mathf.Abs(360 - rotation.eulerAngles.y)%360 );  
        byte[] xRotationBytes = BitConverter.GetBytes(rotation.eulerAngles.x);
        byte[] yRotationBytes = BitConverter.GetBytes(rotation.eulerAngles.z);
        byte[] zRotationBytes = BitConverter.GetBytes(rotation.eulerAngles.y);  
        // Angular velocity.
        byte[] angularVelocityBytesX = BitConverter.GetBytes(angularVelocity[0]);
        byte[] angularVelocityBytesY = BitConverter.GetBytes(angularVelocity[1]);
        byte[] angularVelocityBytesZ = BitConverter.GetBytes(angularVelocity[2]);

        // Copy position and orientation bytes to the packet buffer
        Array.Copy(xPositionBytes, 0, packetBuffer, 0, 4);
        Array.Copy(yPositionBytes, 0, packetBuffer, 4, 4);
        Array.Copy(zPositionBytes, 0, packetBuffer, 8, 4);
        Array.Copy(xRotationBytes, 0, packetBuffer, 12, 4);
        Array.Copy(yRotationBytes, 0, packetBuffer, 16, 4);
        Array.Copy(zRotationBytes, 0, packetBuffer, 20, 4);
        Array.Copy(angularVelocityBytesX, 0, packetBuffer, 24, 4);
        Array.Copy(angularVelocityBytesZ, 0, packetBuffer, 28, 4);
        Array.Copy(angularVelocityBytesY, 0, packetBuffer, 32, 4);

        // Get the current timestamp in microseconds
        uint timestamp = (uint)(Time.time * 1000000);

        // Convert timestamp to bytes
        byte[] timestampBytes = BitConverter.GetBytes(timestamp);

        // Copy timestamp bytes to the packet buffer
        Array.Copy(timestampBytes, 0, packetBuffer, 36, sizeof(uint));

        // Send the UDP packet to the destination
        udpClient.Send(packetBuffer, packetBuffer.Length, ipAddress, port);
    }

    private void OnApplicationQuit()
    {
        // Close the UDP client when the application quits
        udpClient.Close();
    }
}
