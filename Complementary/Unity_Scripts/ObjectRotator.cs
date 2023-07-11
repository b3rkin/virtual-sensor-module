using UnityEngine;

public class ObjectRotator : MonoBehaviour
{
    public float rotationSpeedX = 60f; // Speed of rotation in degrees per second
    public float rotationSpeedY = 60f; // Speed of rotation in degrees per second
    public float rotationSpeedZ = 60f; // Speed of rotation in degrees per second
    public bool rotateX = true; // Whether to rotate on the x-axis
    public bool rotateY = true; // Whether to rotate on the y-axis
    public bool rotateZ = true; // Whether to rotate on the z-axis

    private void FixedUpdate()
    {
        // Calculate the rotation amount based on time and speed
        float rotationAmountX = 0;
        if(rotateX){rotationAmountX = -rotationSpeedX * Time.fixedDeltaTime;}
        float rotationAmountY = 0;
        if(rotateZ){rotationAmountY = -rotationSpeedY * Time.fixedDeltaTime;}
        float rotationAmountZ = 0;
        if(rotateY){rotationAmountZ = -rotationSpeedZ * Time.fixedDeltaTime;}
        // Create a rotation quaternion around the x-axis
        Quaternion rotationQuaternion = Quaternion.Euler(rotationAmountX, rotationAmountY, rotationAmountZ);

        // Apply the rotation to the object's current rotation
        transform.rotation *= rotationQuaternion;
    }
}
