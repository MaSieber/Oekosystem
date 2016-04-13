using UnityEngine;
using System.Collections;

public class HoverPlatform : MonoBehaviour {

    public float movementSpeed = 1.0f;
    public bool bIsActive = true;

    private float currentRadiant = 0.0f;
    private int direction = 1;
	// Use this for initialization
	void Start () {
        currentRadiant = Mathf.PI / 2;
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (!bIsActive)
            return;

        if (currentRadiant >= Mathf.PI)
            direction = -1;
        else if (currentRadiant <= 0)
            direction = 1;

        currentRadiant += Time.deltaTime * direction;
        float transitionY = Mathf.Sin(currentRadiant) * movementSpeed;
        float posY = Mathf.Lerp(-0.4f, 0.4f, transitionY);

        transform.localPosition = new Vector3(0.0f,posY,0.0f);
    }

}

