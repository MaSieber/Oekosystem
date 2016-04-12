using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour {

    public float PlayerSpeed = 1.0f;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {

        float deltaTime = Time.deltaTime;

        Vector3 movement = Vector3.zero;
        if (Input.GetButton("Horizontal"))
        {
            movement.x += PlayerSpeed * deltaTime;
        }
        
        this.gameObject.transform.Translate(movement);
    }
}
