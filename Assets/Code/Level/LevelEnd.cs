using UnityEngine;
using System.Collections;

public class LevelEnd : MonoBehaviour {

    public Vector3 spawnPoint;

	// Use this for initialization
	void Start ()
    {
        

        spawnPoint = GameObject.FindGameObjectWithTag("Player").transform.position;
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnCollisionEnter2D(Collision2D collision)
    {
        collision.gameObject.transform.position = spawnPoint;
    }
}
