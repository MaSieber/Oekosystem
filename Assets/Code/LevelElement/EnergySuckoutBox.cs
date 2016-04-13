using UnityEngine;
using System.Collections;

public class EnergySuckoutBox : MonoBehaviour {

    [HideInInspector]
    public Vector3 startDirection;
    [HideInInspector]
    public bool bDestroying;

    public float despawnTime = 2.0f;

    // Use this for initialization
    void Start () {
        startDirection = Vector3.zero;
        bDestroying = false;
	}
	
	// Update is called once per frame
	void Update ()
    {
	    if (bDestroying)
        {
            startDirection.y -= 9.81f * Time.deltaTime;
            transform.Translate(startDirection);
        }

        if (despawnTime <= 0.0f)
            Destroy(this);
        else
            despawnTime -= Time.deltaTime;
	}
}
