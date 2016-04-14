using UnityEngine;
using System.Collections;

public class Checkpoint : MonoBehaviour {

    public LevelEnd levelEnd;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
    void OnTriggerEnter2D(Collider2D collider)
    {
        Debug.Log(collider.gameObject.name);

        if (collider.gameObject.name == "trigger")
        {
            levelEnd.spawnPoint = collider.gameObject.transform.position;
        }
    }

}
